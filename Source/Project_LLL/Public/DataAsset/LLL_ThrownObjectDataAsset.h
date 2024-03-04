// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseObjectDataAsset.h"
#include "LLL_ThrownObjectDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ThrownObjectDataAsset : public ULLL_BaseObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "데미지 이펙트")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
