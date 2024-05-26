// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_AbilityObjectDataAsset.h"
#include "LLL_PlayerThreateningDeathDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerThreateningDeathDataAsset : public ULLL_AbilityObjectDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "KnockBack", DisplayName = "넉백 파워")
	float KnockBackPower;
};
