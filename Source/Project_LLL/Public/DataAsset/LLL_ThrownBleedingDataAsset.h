// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_ThrownObjectDataAsset.h"
#include "LLL_ThrownBleedingDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ThrownBleedingDataAsset : public ULLL_ThrownObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "출혈 데미지 이펙트")
	TSubclassOf<UGameplayEffect> BleedingDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Collision", DisplayName = "힛 콜리젼 사이즈")
	FVector HitCollisionSize;
};
