// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_InteractiveObjectDataAsset.h"
#include "LLL_GateDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GateDataAsset : public ULLL_InteractiveObjectDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh", DisplayName = "보상 표시용 매시")
	TObjectPtr<UStaticMesh> RewardTextureMesh;

};
