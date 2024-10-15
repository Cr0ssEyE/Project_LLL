// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "LLL_SwordDashDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SwordDashDataAsset : public ULLL_MeleeMonsterDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "대시 데미지 이펙트")
	TSubclassOf<UGameplayEffect> DashDamageEffect;
};
