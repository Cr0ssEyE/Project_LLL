// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Monster/MeleeMonster/LLL_MGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
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

		BP_ApplyGameplayEffectToTarget(TargetDataHandle, AttackDamageEffect);
	}
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
