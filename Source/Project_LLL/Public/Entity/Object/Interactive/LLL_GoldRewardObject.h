// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "LLL_GoldRewardObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_GoldRewardObject : public ALLL_RewardObject
{
	GENERATED_BODY()

public:
	virtual void SetInformation(const FRewardDataTable* Data, const uint32 Index = 0) override;
	virtual void InteractiveEvent(AActor* InteractedActor) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	uint8 RewardValue;
};
