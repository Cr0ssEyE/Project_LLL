// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_KnockBackCollisionCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "Util/LLL_MathHelper.h"

ULLL_PGA_KnockBackCollisionCheck:: ULLL_PGA_KnockBackCollisionCheck()
{
	
}

void ULLL_PGA_KnockBackCollisionCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ULLL_AT_WaitTargetData* TargetDataTask = ULLL_AT_WaitTargetData::CreateTask(this, ALLL_MonsterBase::StaticClass(), false, false);
	TargetDataTask->TargetDataReceivedDelegate.AddDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnTraceResultCallBack);
	TargetDataTask->ReadyForActivation();
}

void ULLL_PGA_KnockBackCollisionCheck::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (!KnockBackedCharacters.IsEmpty())
	{
		for (auto Character : KnockBackedCharacters)
		{
			if (IsValid(Character) && Character->OtherActorCollidedDelegate.IsAlreadyBound(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack))
			{
				Character->OtherActorCollidedDelegate.RemoveDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack);
			}
		}
		KnockBackedCharacters.Empty();
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_KnockBackCollisionCheck::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);;
	}

	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	const float CollideCheckTime = FLLL_MathHelper::CalculatePlayerKnockBackCollisionCheckEndApproximation(PlayerAttributeSet->GetKnockBackPower());
	for (auto Actor : TargetDataHandle.Data[0]->GetActors())
	{
		const ILLL_KnockBackInterface* ActorCanKnockBacked = Cast<ILLL_KnockBackInterface>(Actor);
		if (!ActorCanKnockBacked)
		{
			continue;
		}

		ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(Actor);
		if (Character && !KnockBackedCharacters.Contains(Character))
		{
			Character->OtherActorCollidedDelegate.AddDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack);
			KnockBackedCharacters.Emplace(Character);
		}
	}

	FTimerHandle CollideCheckTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CollideCheckTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}), CollideCheckTime, false);
}

void ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack(AActor* HitActor, AActor* OtherActor)
{
	if (OtherActor == GetAvatarActorFromActorInfo())
	{
		return;
	}
	
	// 넉백당한 대상에 대한 처리
	ALLL_BaseCharacter* HitCharacter = Cast<ALLL_BaseCharacter>(HitActor);
	if (!IsValid(HitCharacter))
	{
		return;
	}
	
	HitCharacter->GetCharacterMovement()->Velocity = FVector::Zero();
	HitCharacter->OtherActorCollidedDelegate.RemoveDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack);
	const FGameplayAbilityTargetDataHandle HitActorHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitCharacter);
	BP_ApplyGameplayEffectToTarget(HitActorHandle, CollideCauserApplyEffect, CurrentEventData.EventMagnitude);
	
	// 넉백당한 대상에 충돌한 대상에 대한 처리
	if (Cast<IAbilitySystemInterface>(OtherActor))
	{
		const FGameplayAbilityTargetDataHandle OtherActorHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
		BP_ApplyGameplayEffectToTarget(OtherActorHandle, CollideTargetApplyEffect, CurrentEventData.EventMagnitude);
	}

	if (KnockBackedCharacters.Contains(HitCharacter))
	{
		KnockBackedCharacters.Remove(HitCharacter);
		if (KnockBackedCharacters.IsEmpty())
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}
