// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_KnockBackDamageCalculate.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

ULLL_CC_KnockBackDamageCalculate::ULLL_CC_KnockBackDamageCalculate()
{
	bUseKnockBackInfluence = false;
}

float ULLL_CC_KnockBackDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(PlayerCharacter))
	{
		return Result;
	}
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	const float KnockBackPower = PlayerAttributeSet->GetKnockBackPower() * 0.01f;
	const float KnockBackDamage = PlayerAttributeSet->GetKnockBackDamage();
	const float KnockBackInfluence = bUseKnockBackInfluence? PlayerAttributeSet->GetKnockBackInfluence() : 1.f;

	Result = KnockBackPower * KnockBackDamage * KnockBackInfluence;
	return Result;
}
