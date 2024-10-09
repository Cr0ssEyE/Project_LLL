// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LLL_CC_AttackDamageCalculate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CC_AttackDamageCalculate : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
