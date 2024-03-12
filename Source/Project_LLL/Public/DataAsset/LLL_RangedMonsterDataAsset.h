// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MonsterBaseDataAsset.h"
#include "LLL_RangedMonsterDataAsset.generated.h"

class ALLL_ThrownObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RangedMonsterDataAsset : public ULLL_MonsterBaseDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Thrown Object", DisplayName = "투사체")
	TSubclassOf<ALLL_ThrownObject> ThrownObjectClass;
};
