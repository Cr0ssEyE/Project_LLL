// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/ComboSystem/LLL_PGA_ChangeComboAmplify_ByCombo.h"

#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_ChangeComboAmplify_ByCombo::ULLL_PGA_ChangeComboAmplify_ByCombo()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboAmplifyLevel = 1;
}

void ULLL_PGA_ChangeComboAmplify_ByCombo::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ApplySkillGaugeAmplify();
}

void ULLL_PGA_ChangeComboAmplify_ByCombo::ApplySkillGaugeAmplify()
{
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	const uint32 CurrentComboCount = PlayerAttributeSet->GetCurrentComboCount();
	const uint32 NextAmplifyNeededComboCount = ComboAmplifyChangeSection.Eval(CurrentComboAmplifyLevel, ComboAmplifyChangeSection.RowName.ToString());
	if(CurrentComboCount >= NextAmplifyNeededComboCount)
	{
		
	}
	BP_ApplyGameplayEffectToOwner(UpdateComboAmplifyEffect, CurrentComboAmplifyLevel);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
