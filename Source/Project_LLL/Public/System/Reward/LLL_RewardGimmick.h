// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_RewardGimmick.generated.h"

struct FTestRewardDataTable;
struct FTestAbilityDataTable;
class ULLL_RewardDataTable;
class ULLL_TestAbilityDataTable;
class ALLL_AbilityRewardObject;

UCLASS()
class PROJECT_LLL_API ALLL_RewardGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_RewardGimmick();

	FORCEINLINE FTestRewardDataTable* GetRewardData(uint8 index) { return &RewardData[index]; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetRewardToGate(ALLL_GateObject* Gate);

	void SetAbilityToReward(ALLL_AbilityRewardObject* RewardObject);
	
	UFUNCTION()
	void SetRewardButtons();

	void SetDataTable();
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestRewardDataTable> RewardData;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestAbilityDataTable> AbilityData;

	FTestAbilityDataTable* ButtonAbilityData1;
	FTestAbilityDataTable* ButtonAbilityData2;
	FTestAbilityDataTable* ButtonAbilityData3;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsButtonEventSetup : 1;

	UFUNCTION()
	void ClickFirstButton();

	UFUNCTION()
	void ClickSecondButton();

	UFUNCTION()
	void ClickThirdButton();

};
