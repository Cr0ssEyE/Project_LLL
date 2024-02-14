// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Constant/LLL_GameplayTags.h"
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
	if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		// 맞은 액터 갯수만큼 콤보 수 증가
		float Magnitude = TargetDataHandle.Data[0]->GetActors().Num();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(ComboStackEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_COMBO_ADDITIVE, Magnitude);
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle);
		}
		
		BP_ApplyGameplayEffectToTarget(TargetDataHandle, AttackDamageEffect);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
