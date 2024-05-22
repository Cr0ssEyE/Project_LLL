// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackHitCheck.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Constant/LLL_GameplayTags.h"
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
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	{
		return;
	}
	
	// 맞은 액터 갯수만큼 콤보 수 증가
	const int Magnitude = TargetDataHandle.Data[0]->GetActors().Num();
	const FGameplayEffectSpecHandle ComboEffectSpecHandle = MakeOutgoingGameplayEffectSpec(ComboStackEffect, CurrentEventData.EventMagnitude);
	if (ComboEffectSpecHandle.IsValid())
	{
		ComboEffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_COMBO_ADDITIVE, Magnitude);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, ComboEffectSpecHandle);
	}

	// 맞은 액터 갯수만큼 스킬 게이지 증가
	const FGameplayEffectSpecHandle SkillGaugeEffectSpecHandle = MakeOutgoingGameplayEffectSpec(IncreaseSkillGaugeEffect, CurrentEventData.EventMagnitude);
	if (SkillGaugeEffectSpecHandle.IsValid())
	{
		SkillGaugeEffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_SKILL_GAUGE_ADDITIVE, Magnitude);
		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, SkillGaugeEffectSpecHandle);
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
}

void ULLL_PGA_AttackHitCheck::OnTraceEndCallBack(FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
