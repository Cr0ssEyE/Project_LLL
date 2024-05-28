// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_KnockBackDamageCalculate.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

ULLL_CC_KnockBackDamageCalculate::ULLL_CC_KnockBackDamageCalculate()
{
	bUsePowKnockBackRate = false;
}

float ULLL_CC_KnockBackDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(PlayerCharacter))
	{
		return Result;
	}
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	const float KnockBackPower = FLLL_MathHelper::CalculateKnockBackPower(PlayerAttributeSet, KnockBackAmplifyValueRowHandle.Eval(Spec.GetLevel(), TEXT("Error!!!")));
	const float MainKnockBackRate = PlayerAttributeSet->GetKnockBackRate();
	const float SubKnockBackRate = bUsePowKnockBackRate? PlayerAttributeSet->GetKnockBackRate() : 1.f;

	Result = KnockBackPower * MainKnockBackRate * SubKnockBackRate;
	return Result;
}
