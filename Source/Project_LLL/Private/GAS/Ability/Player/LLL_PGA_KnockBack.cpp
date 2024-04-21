// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_KnockBack.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Task/LLL_AT_Trace.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "Util/LLL_MathHelper.h"

ULLL_PGA_KnockBack::ULLL_PGA_KnockBack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	KnockBackMultiplier = 1.f;
}

void ULLL_PGA_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ULLL_AT_WaitTargetData* TargetDataTask = ULLL_AT_WaitTargetData::CreateTask(this, ALLL_MonsterBase::StaticClass(), false, false);
	TargetDataTask->TargetDataReceivedDelegate.AddDynamic(this, &ULLL_PGA_KnockBack::OnTraceResultCallBack);
	TargetDataTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayTagAdded* TraceEndTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_GAS_ATTACK_HIT_CHECK_COMPLETE);
	TraceEndTask->Added.AddDynamic(this, &ULLL_PGA_KnockBack::OnTraceEndCallBack);
	TraceEndTask->ReadyForActivation();
}

void ULLL_PGA_KnockBack::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		return;
	}
	
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	const FVector AvatarLocation = CurrentActorInfo->AvatarActor->GetActorLocation();
	for (auto Actor : TargetDataHandle.Data[0]->GetActors())
	{
		// 초기 구현은 MovementComponent의 LaunchCharacter 기반 물리 넉백으로 구현. 추후 방향성에 따른 수정 예정
		ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Actor);
		if (KnockBackActor)
		{
			const FVector LaunchDirection = (Actor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
			FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, PlayerCharacterAttributeSet->GetKnockBackPower() * KnockBackMultiplier);
			KnockBackActor->AddKnockBackVelocity(LaunchVelocity);
		}

		// 만약 넉백 당하지는 않지만 넉백 관련 이벤트가 있는 대상일 경우를 위해 위와 별도 처리
		if (Cast<IAbilitySystemInterface>(Actor))
		{
			BP_ApplyGameplayEffectToTarget(TargetDataHandle, KnockBackEffect);
		}
	}
}

void ULLL_PGA_KnockBack::OnTraceEndCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
