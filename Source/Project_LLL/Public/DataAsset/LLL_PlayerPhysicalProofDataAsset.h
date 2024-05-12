// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SkillObjectDataAsset.h"
#include "LLL_PlayerPhysicalProofDataAsset.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerPhysicalProofDataAsset : public ULLL_SkillObjectDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Niagara", DisplayName = "파티클")
	TObjectPtr<UNiagaraSystem> Particle;
};
