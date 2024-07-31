// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "LLL_AbilityRewardObject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilityRewardInteractionDelegate, EAbilityCategory);
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_AbilityRewardObject : public ALLL_RewardObject
{
	GENERATED_BODY()

public:
	virtual void SetInformation(const FRewardDataTable* Data) override;
	virtual void InteractiveEvent() override;

	void SetAbilityCategory(EAbilityCategory AbilityData);
	
	FOnAbilityRewardInteractionDelegate* AbilityRewardInteractionDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	EAbilityCategory AbilityCategory;

	
};
