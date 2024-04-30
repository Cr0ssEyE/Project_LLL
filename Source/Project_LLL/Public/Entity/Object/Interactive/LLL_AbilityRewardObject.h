// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "LLL_AbilityRewardObject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAbilityRewardInteractionDelegate);
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_AbilityRewardObject : public ALLL_RewardObject
{
	GENERATED_BODY()

public:
	FORCEINLINE FOnAbilityRewardInteractionDelegate* GetAbilityRewardInteractionDelegate() { return AbilityRewardInteractionDelegate; }
	
	virtual void SetInformation(FTestRewardDataTable* Data) override;

	virtual void InteractiveEvent() override;

	void SetAbilityData(TArray<FTestAbilityDataTable> AbilityData);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FTestAbilityDataTable> RewardAbilityData;

	FOnAbilityRewardInteractionDelegate* AbilityRewardInteractionDelegate;
};
