// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"
#include "LLL_RangedMonsterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RangedMonsterAttributeSet : public ULLL_MonsterAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_RangedMonsterAttributeSet, PredictionRate);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData PredictionRate;
};
