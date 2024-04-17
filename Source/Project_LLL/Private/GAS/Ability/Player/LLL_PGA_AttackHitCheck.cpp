// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "GAS/Ability/Share/LLL_GA_KnockBack.h"
#include "GAS/Task/LLL_AT_Trace.h"

ULLL_PGA_AttackHitCheck::ULLL_PGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentLevel = TriggerEventData->EventMagnitude;

	ULLL_AT_Trace* AttackTraceTask = ULLL_AT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->TaskOnCompleteDelegate.AddDynamic(this, &ULLL_PGA_AttackHitCheck::OnTraceResultCallBack);
	AttackTraceTask->ReadyForActivation();
	
}

void ULLL_PGA_AttackHitCheck::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	// 맞은 액터 갯수만큼 콤보 수 증가
	const int Magnitude = TargetDataHandle.Data[0]->GetActors().Num();
	const FGameplayEffectSpecHandle ComboEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ComboStackEffect, CurrentLevel);
	if (ComboEffectSpecHandle.IsValid())
	{
		ComboEffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_COMBO_ADDITIVE, Magnitude);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, ComboEffectSpecHandle);
	}

	// 맞은 액터 갯수만큼 스킬 게이지 증가
	const FGameplayEffectSpecHandle SkillGaugeEffectSpecHandle = MakeOutgoingGameplayEffectSpec(IncreaseSkillGaugeEffect, CurrentLevel);
	if (SkillGaugeEffectSpecHandle.IsValid())
	{
		SkillGaugeEffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_SKILL_GAUGE_ADDITIVE, Magnitude);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SkillGaugeEffectSpecHandle);
	}
		
	BP_ApplyGameplayEffectToTarget(TargetDataHandle, AttackDamageEffect);
	BP_ApplyGameplayEffectToTarget(TargetDataHandle, GiveTagEffect);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
