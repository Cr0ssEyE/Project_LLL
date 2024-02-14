// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Monster/LLL_MGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Ability/Player/LLL_PGA_AttackHitCheck.h"
#include "GAS/Task/LLL_AT_Trace.h"

void ULLL_MGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ULLL_AT_Trace* AttackTraceTask = ULLL_AT_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->TaskOnCompleteDelegate.AddDynamic(this, &ULLL_MGA_AttackHitCheck::OnTraceResultCallBack);
	AttackTraceTask->ReadyForActivation();
}

void ULLL_MGA_AttackHitCheck::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDataHandle);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
