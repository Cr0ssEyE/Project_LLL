// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_OrawaveMeleeDataAsset.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "LLL_OrawaveMelee.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_OrawaveMelee : public ALLL_MeleeMonster
{
	GENERATED_BODY()

public:
	ALLL_OrawaveMelee();

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_OrawaveMeleeDataAsset> OrawaveMeleeDataAsset;
};
