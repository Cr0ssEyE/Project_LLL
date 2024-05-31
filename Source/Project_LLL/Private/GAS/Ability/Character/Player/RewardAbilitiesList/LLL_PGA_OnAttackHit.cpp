// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/RewardAbilitiesList/LLL_PGA_OnAttackHit.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Task/LLL_AT_WaitTargetData.h"
#include "Util/LLL_AbilityDataHelper.h"

void ULLL_PGA_OnAttackHit::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (TriggerRequiredTag.IsValid() && !TriggerEventData->InstigatorTags.HasTag(TriggerRequiredTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	if (!UAbilitySystemBlueprintLibrary::TargetDataHasActor(CurrentEventData.TargetData, 0))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	ApplyEffectWhenHit();
}

void ULLL_PGA_OnAttackHit::ApplyEffectWhenHit()
{
	const ULLL_ExtendedGameplayEffect* Effect = Cast<ULLL_ExtendedGameplayEffect>(OnAttackHitEffect.GetDefaultObject());
	const FGameplayEffectSpecHandle EffectHandle = MakeOutgoingGameplayEffectSpec(OnAttackHitEffect, GetAbilityLevel());

	const float MagnitudeValue = (AbilityData->AbilityValue + AbilityData->ChangeValue * GetAbilityLevel()) / static_cast<uint32>(AbilityData->AbilityValueType);
	EffectHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_EFFECT_VALUE, MagnitudeValue);
	
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
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
