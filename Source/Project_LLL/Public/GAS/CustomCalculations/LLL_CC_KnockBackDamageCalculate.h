// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LLL_CC_KnockBackDamageCalculate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CC_KnockBackDamageCalculate : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	ULLL_CC_KnockBackDamageCalculate();

protected:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "모션 별 넉백 배율 테이블 열")
	FCurveTableRowHandle KnockBackAmplifyValueRowHandle;
	
	UPROPERTY(EditAnywhere, DisplayName = "넉백 비율 제곱 계산 여부")
	uint8 bUsePowKnockBackRate : 1;
	
};
