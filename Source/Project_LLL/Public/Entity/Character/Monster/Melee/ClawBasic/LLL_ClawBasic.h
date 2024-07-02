// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ClawBasicDataAsset.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"
#include "LLL_ClawBasic.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_ClawBasic : public ALLL_MeleeMonster
{
	GENERATED_BODY()

public:
	ALLL_ClawBasic();

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ClawBasicDataAsset> ClawBasicDataAsset;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LeftClawMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> RightClawMeshComponent;
};
