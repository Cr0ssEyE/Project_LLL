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

UCLASS()
class PROJECT_LLL_API ALLL_RewardGimmick : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_RewardGimmick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetRewardToDoor(ALLL_GateObject* Gate);
	
	UFUNCTION()
	void SetRewardButtons();
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestRewardDataTable> RewardData;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestAbilityDataTable> AbilityData;

	FTestAbilityDataTable* ButtonAbilityData1;
	FTestAbilityDataTable* ButtonAbilityData2;
	FTestAbilityDataTable* ButtonAbilityData3;

	UFUNCTION()
	void ClickFirstButton();

	UFUNCTION()
	void ClickSecondButton();

	UFUNCTION()
	void ClickThirdButton();

};
