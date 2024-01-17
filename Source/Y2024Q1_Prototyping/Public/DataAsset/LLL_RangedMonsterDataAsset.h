// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "LLL_RangedMonsterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_RangedMonsterDataAsset : public ULLL_MonsterBaseDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 예측률")
	float PredictionRate;
};
