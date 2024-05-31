// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/ASC/LLL_BaseASC.h"
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
			K2_ApplyGameplayEffectSpecToOwner(HitCountEffectSpecHandle);
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
	// 아래와 같이 복수의 데이터 전달 가능
	PayloadData.TargetData = TargetDataHandle;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_ATTACK_HIT_CHECK_SUCCESS, PayloadData);
}

void ULLL_PGA_AttackHitCheck::OnTraceEndCallBack(FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
