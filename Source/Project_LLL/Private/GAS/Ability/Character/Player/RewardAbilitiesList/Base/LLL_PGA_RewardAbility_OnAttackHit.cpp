// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbility_OnAttackHit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"

void ULLL_PGA_RewardAbility_OnAttackHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ULLL_AT_WaitTargetData* TargetDataTask = ULLL_AT_WaitTargetData::CreateTask(this, ALLL_MonsterBase::StaticClass(), false, false);
	TargetDataTask->TargetDataReceivedDelegate.AddDynamic(this, &ULLL_PGA_RewardAbility_OnAttackHit::OnTraceResultCallBack);
	TargetDataTask->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* TraceEndTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_GAS_ATTACK_HIT_CHECK_COMPLETE);
	TraceEndTask->Added.AddDynamic(this, &ULLL_PGA_RewardAbility_OnAttackHit::OnTraceEndCallBack);
	TraceEndTask->ReadyForActivation();
}

void ULLL_PGA_RewardAbility_OnAttackHit::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		return;
	}

	for (auto Effect : OnAttackHitEffects)
	{
		BP_ApplyGameplayEffectToTarget(TargetDataHandle, Effect, GetAbilityLevel());
	}
}

void ULLL_PGA_RewardAbility_OnAttackHit::OnTraceEndCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
