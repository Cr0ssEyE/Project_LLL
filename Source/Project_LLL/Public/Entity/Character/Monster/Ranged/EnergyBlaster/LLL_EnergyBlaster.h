// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_EnergyBlasterDataAsset.h"
#include "Entity/Character/Monster/Ranged/Base/LLL_RangedMonster.h"
#include "Interface/LLL_KnockBackInterface.h"
#include "LLL_EnergyBlaster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_EnergyBlaster : public ALLL_RangedMonster, public ILLL_KnockBackInterface
{
	GENERATED_BODY()

public:
	ALLL_EnergyBlaster();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_EnergyBlasterDataAsset> EnergyBlasterDataAsset;
};
