// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "LLL_SwordDash.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SwordDash : public ALLL_MeleeMonster, public ILLL_DashMonsterInterface
{
	GENERATED_BODY()

public:
	ALLL_SwordDash();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Dash() override;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;
};
