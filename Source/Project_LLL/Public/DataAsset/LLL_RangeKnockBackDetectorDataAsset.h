// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "LLL_RangeKnockBackDetectorDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RangeKnockBackDetectorDataAsset : public ULLL_AbilityObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "오버랩 콜리젼 크기")
	FVector OverlapCollisionScale;
};
