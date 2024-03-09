// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_OrawaveDataAsset.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "LLL_Orawave.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_Orawave : public ALLL_MeleeMonster
{
	GENERATED_BODY()

public:
	ALLL_Orawave();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_OrawaveDataAsset> OrawaveMeleeDataAsset;
};
