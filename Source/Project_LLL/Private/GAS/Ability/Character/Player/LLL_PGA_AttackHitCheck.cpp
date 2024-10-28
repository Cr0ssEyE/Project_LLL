// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ASC/LLL_BaseASC.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"

ULLL_PGA_AttackHitCheck::ULLL_PGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	TraceTask = ULLL_AT_Trace::CreateTask(this, TargetActorClass, false);
	TraceTask->TaskOnCompleteDelegate.AddDynamic(this, &ULLL_PGA_AttackHitCheck::OnTraceResultCallBack);
	TraceTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* TraceEndTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_ATTACK_HIT_CHECK_COMPLETE);
	TraceEndTask->EventReceived.AddDynamic(this, &ULLL_PGA_AttackHitCheck::OnTraceEndCallBack);
	TraceEndTask->ReadyForActivation();
}

void ULLL_PGA_AttackHitCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	TraceTask->EndTask();
	GetAbilitySystemComponentFromActorInfo_Checked()->RemoveLooseGameplayTag(TAG_GAS_ATTACK_HIT_CHECK_COMPLETE);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_AttackHitCheck::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	// 디버그 찍어보니 이상해서 살펴보니 이거 실수로 빠졌나봐요
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]()
	{
		if(IsValid(TraceTask) && !bIsAbilityEnding)
		{
			TraceTask->Activate();
		}
	}));
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		return;
	}
	
	// 맞은 액터 갯수만큼 콤보 수 증가
	const int Magnitude = TargetDataHandle.Data[0]->GetActors().Num();

	if (!CheckHitCountEffects.IsEmpty())
	{
		for (const auto HitCountEffect : CheckHitCountEffects)
		{
			const FGameplayEffectSpecHandle HitCountEffectSpecHandle = MakeOutgoingGameplayEffectSpec(HitCountEffect, CurrentEventData.EventMagnitude);
			if (!HitCountEffectSpecHandle.IsValid())
			{
				continue;
			}
			HitCountEffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ATTACK_HIT_COUNT, Magnitude);
			BP_ApplyGameplayEffectToOwner(HitCountEffect);
		}
	}

	BP_ApplyGameplayEffectToTarget(TargetDataHandle, AttackDamageEffect, CurrentEventData.EventMagnitude);
	BP_ApplyGameplayEffectToTarget(TargetDataHandle, GiveTagEffect);

	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]()
	{
		if (!IsValid(GetAbilitySystemComponentFromActorInfo()))
		{
			return;
		}
		
		Cast<ULLL_BaseASC>(GetAbilitySystemComponentFromActorInfo_Checked())->ReceiveTargetData(this, TargetDataHandle);
	}));

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	
	FGameplayEventData PayloadData;
	FGameplayTagContainer TriggerTags;
	for (auto Trigger : AbilityTriggers)
	{
		TriggerTags.AddTag(Trigger.TriggerTag);
	}
	PayloadData.Instigator = GetAvatarActorFromActorInfo();
	PayloadData.InstigatorTags.AppendTags(GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags());
	PayloadData.InstigatorTags.AppendTags(TriggerTags);
	PayloadData.TargetData = TargetDataHandle;
	PayloadData.EventMagnitude = CurrentEventData.EventMagnitude;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_ATTACK_HIT_CHECK_SUCCESS, PayloadData);
	/*if (UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent())
	{
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_MOVE_FASTER))
		{
			PlayerASC->ExecuteGameplayCue(TAG_GAS_CUE_MOVE_FASTER);
		}
	}*/
	
	for (auto TargetActor : TargetDataHandle.Data[0]->GetActors())
	{
		// 날아온 몬스터 받아치기
		if (ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(TargetActor))
		{
			ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(Monster->GetOwner());
			UCharacterMovementComponent* MonsterMovement = Monster->GetCharacterMovement();
			if (IsValid(ManOfStrength) && MonsterMovement->MovementMode == MOVE_Flying)
			{
				UE_LOG(LogTemp, Log, TEXT("%s가 %s에게 받아치기"), *Player->GetName(), *ManOfStrength->GetName())
				Monster->GetMesh()->SetCollisionProfileName(CP_THREW_MONSTER_BY_PLAYER);
				Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_THREW_MONSTER_BY_PLAYER);
				Monster->SetOwner(Player);

				const UAbilitySystemComponent* ManOfStrengthASC = ManOfStrength->GetAbilitySystemComponent();
				const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrengthASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));

				const float Speed = MonsterAttributeSet->GetManOfStrengthThrowSpeed();
				const FVector Direction = (ManOfStrength->GetActorLocation() - Monster->GetActorLocation()).GetSafeNormal();
				MonsterMovement->Deactivate();
				MonsterMovement->Activate();
				MonsterMovement->Velocity = Direction * Speed * 2.0f;
			}
		}

		if (ALLL_BreakableObjectBase* BreakableObject = Cast<ALLL_BreakableObjectBase>(TargetActor))
		{
			BreakableObject->ReceivePlayerAttackOrKnockBackedMonster();
		}
	}
}

void ULLL_PGA_AttackHitCheck::OnTraceEndCallBack(FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
