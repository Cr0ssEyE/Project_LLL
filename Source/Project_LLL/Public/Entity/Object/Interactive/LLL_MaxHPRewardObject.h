// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "LLL_MaxHPRewardObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MaxHPRewardObject : public ALLL_RewardObject
{
	GENERATED_BODY()


public:

	virtual void SetInformation(FRewardDataTable* Data) override;

	virtual void InteractiveEvent() override;
};
