// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_AbilityObjectDataAsset.h"
#include "LLL_PlayerFeatherStormDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerFeatherStormDataAsset : public ULLL_AbilityObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "파티클")
	TObjectPtr<UNiagaraSystem> Particle;
};
