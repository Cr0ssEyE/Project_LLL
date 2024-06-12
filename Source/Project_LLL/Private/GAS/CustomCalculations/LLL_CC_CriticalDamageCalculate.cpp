// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CustomCalculations/LLL_CC_CriticalDamageCalculate.h"

float ULLL_CC_CriticalDamageCalculate::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	float Result = Super::CalculateBaseMagnitude_Implementation(Spec);

	Result *= FMath::RandBool() ? 1 : 10;
	
	return Result;
}
