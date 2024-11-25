// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_BombSkullDataAsset.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"
#include "LLL_BombSkull.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_BombSkull : public ALLL_MeleeMonster
{
	GENERATED_BODY()

public:
	ALLL_BombSkull();

protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BombSkullDataAsset> BombSkullDataAsset;
};
