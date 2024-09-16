// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_CriticalDamageCalculate.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

float ULLL_CC_CriticalDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(PlayerCharacter))
	{
		return Result;
	}
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	const FGameplayTagContainer EffectTagContainer = Spec.Def->GetAssetTags();
	float DamageAmplifyValue = 1.f;
	if (EffectTagContainer.HasTag(TAG_GAS_ATTACK_HIT_CHECK_MELEE))
	{
		DamageAmplifyValue = PlayerAttributeSet->GetBaseAttackDamageAmplifyByOther();
	}
	
	Result = FLLL_MathHelper::CalculateCriticalDamage(PlayerAttributeSet, PlayerAttributeSet->GetOffencePower() * DamageAmplifyValue);
	
	return Result;
}
