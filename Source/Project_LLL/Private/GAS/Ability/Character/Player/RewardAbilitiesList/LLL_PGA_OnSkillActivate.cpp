﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnSkillActivate.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_GameplayTags.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "Util/LLL_AbilityDataHelper.h"

ULLL_PGA_OnSkillActivate::ULLL_PGA_OnSkillActivate() :
EffectApplyTarget(EEffectApplyTarget::Self)
{
	
}

void ULLL_PGA_OnSkillActivate::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_BULLET_TIME_END, nullptr, true);
	EventTask->EventReceived.AddDynamic(this, &ULLL_PGA_OnSkillActivate::OnSkillDeactivatedCallBack);
	EventTask->ReadyForActivation();

	if (OnActivateSuccessEffect)
	{
		ApplyEffectWhenActivateSuccess();
	}
}

void ULLL_PGA_OnSkillActivate::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_OnSkillActivate::ApplyEffectWhenActivateSuccess()
{
	ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(OnActivateSuccessEffect.GetDefaultObject());
	const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(OnActivateSuccessEffect, GetAbilityLevel());

	const float ChangeableValue = (AbilityData->AbilityValue + AbilityData->ChangeValue * GetAbilityLevel()) / static_cast<uint32>(AbilityData->AbilityValueType);
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, ChangeableValue);

	const float UnChangeableValue = AbilityData->UnchangeableValue;
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_UNCHANGEABLE_VALUE, UnChangeableValue);
	
	if (AbilityTags.HasTag(TAG_GAS_ABNORMAL_STATUS))
	{
		FLLL_AbilityDataHelper::SetAbnormalStatusAbilityDuration(this, EffectHandle.Data);
	}
	
	if (Effect->GetEffectApplyTarget() == EEffectApplyTarget::Self)
	{
		K2_ApplyGameplayEffectSpecToOwner(EffectHandle);
	}
	else
	{
		K2_ApplyGameplayEffectSpecToTarget(EffectHandle, CurrentEventData.TargetData);
	}
}

void ULLL_PGA_OnSkillActivate::OnSkillDeactivatedCallBack(FGameplayEventData EventData)
{
	GetAbilitySystemComponentFromActorInfo_Checked()->RemoveActiveGameplayEffectBySourceEffect(OnActivateSuccessEffect, GetAbilitySystemComponentFromActorInfo_Checked());
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
