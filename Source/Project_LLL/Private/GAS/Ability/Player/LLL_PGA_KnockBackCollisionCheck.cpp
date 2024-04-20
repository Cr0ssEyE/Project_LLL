// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_KnockBackCollisionCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Constant/LLL_AnimMontageSlotName.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "Interface/LLL_EntityInterface.h"

ULLL_PGA_KnockBackCollisionCheck::ULLL_PGA_KnockBackCollisionCheck()
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

	for (auto Actor : TargetDataHandle.Data[0]->GetActors())
	{
		const ILLL_KnockBackInterface* ActorCanKnockBacked = Cast<ILLL_KnockBackInterface>(Actor);
		if (!ActorCanKnockBacked)
		{
			continue;
		}

		if (ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(Actor))
		{
			Character->OtherActorCollidedDelegate.AddDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack);
			Character->GetCharacterAnimInstance()->MontageEndedEnhancedDelegate.AddDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnDamagedMontageEndedCallBack);
			KnockBackedCharacters.Emplace(Character);
		}
	}
}

void ULLL_PGA_KnockBackCollisionCheck::OnDamagedMontageEndedCallBack(ALLL_BaseCharacter* Character, UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage->GetGroupName() == ANIM_SLOT_DAMAGED && KnockBackedCharacters.Contains(Character))
	{
		KnockBackedCharacters.Remove(Character);

		if (KnockBackedCharacters.IsEmpty())
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		}
	}
}

void ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack(AActor* HitActor, AActor* OtherActor)
{
	// 넉백당한 대상에 대한 처리
	ALLL_BaseCharacter* HitCharacter = Cast<ALLL_BaseCharacter>(HitActor);
	if (IsValid(HitCharacter))
	{
		HitCharacter->GetCharacterMovement()->Velocity = FVector::Zero();
		HitCharacter->OtherActorCollidedDelegate.RemoveDynamic(this, &ULLL_PGA_KnockBackCollisionCheck::OnOtherActorCollidedCallBack);
		const FGameplayAbilityTargetDataHandle HitActorHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(HitCharacter);
		BP_ApplyGameplayEffectToTarget(HitActorHandle, CollideCauserApplyEffect);
	}

	// 넉백당한 대상에 충돌한 대상에 대한 처리
	const IAbilitySystemInterface* OtherActorHasGAS = Cast<IAbilitySystemInterface>(OtherActor);
	if (OtherActorHasGAS)
	{
		const FGameplayAbilityTargetDataHandle OtherActorHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
		BP_ApplyGameplayEffectToTarget(OtherActorHandle, CollideTargetApplyEffect);
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
