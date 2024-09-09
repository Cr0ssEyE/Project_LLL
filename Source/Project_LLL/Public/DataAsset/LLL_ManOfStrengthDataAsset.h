// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_BossMonsterDataAsset.h"
#include "LLL_ManOfStrengthDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ManOfStrengthDataAsset : public ULLL_BossMonsterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "대시 데미지 이펙트")
	TSubclassOf<UGameplayEffect> DashDamageEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "투척 데미지 이펙트")
	TSubclassOf<UGameplayEffect> ThrowDamageEffect;
};
