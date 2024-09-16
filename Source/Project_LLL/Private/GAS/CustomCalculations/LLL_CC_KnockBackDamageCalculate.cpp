// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_KnockBackDamageCalculate.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

float ULLL_CC_KnockBackDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);
	
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Spec.GetEffectContext().GetInstigator());
	if (!IsValid(Player))
	{
		return Result;
	}
	
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	Result = PlayerAttributeSet->GetKnockBackOffencePower() * PlayerAttributeSet->GetKnockBackOffencePowerRate();

	UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_INCREASE_KNOCK_BACK_DAMAGE_BY_ENURIA_COUNT))
	{
		Result *= 1 + Player->GetIncreaseKnockBackDamageByEnuriaCountDamageRate() * Player->GetEnuriaCount();
	}
	
	return Result;
}
