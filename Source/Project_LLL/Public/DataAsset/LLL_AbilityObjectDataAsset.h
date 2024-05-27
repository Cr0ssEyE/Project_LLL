// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseObjectDataAsset.h"
#include "LLL_AbilityObjectDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AbilityObjectDataAsset : public ULLL_BaseObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "오버랩 콜리젼 사이즈")
	FVector OverlapCollisionSize;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "데미지 이펙트")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
