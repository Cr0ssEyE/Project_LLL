// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_RewardGimmick.generated.h"

class UAbilitySystemComponent;
class ULLL_ExtendedGameplayEffect;
class ALLL_GateObject;
struct FRewardDataTable;
struct FAbilityDataTable;
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

	FORCEINLINE FRewardDataTable* GetRewardData(uint8 index) { return &RewardData[index]; }
	FORCEINLINE void InformMapGimmickIsExist() { bMapGimmickIsExist = true; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetRewardToGate(ALLL_GateObject* Gate);
	
	UFUNCTION()
	void SetRewardButtons();

	void SetDataTable();

	
protected:
	UFUNCTION()
	void ClickFirstButton();

	UFUNCTION()
	void ClickSecondButton();

	UFUNCTION()
	void ClickThirdButton();
	
	void ClickButtonEvent(FAbilityDataTable* ButtonAbilityData);

	UFUNCTION()
	void ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects);
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FRewardDataTable> RewardData;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FAbilityDataTable> AbilityData;

	uint32 FirstButtonIndex;
	uint32 SecondButtonIndex;
	uint32 ThirdButtonIndex;
	TArray<uint32> GettenIndexArray; 
	
	FAbilityDataTable* ButtonAbilityData1;
	FAbilityDataTable* ButtonAbilityData2;
	FAbilityDataTable* ButtonAbilityData3;
	TArray<FAbilityDataTable*> ButtonAbilityDataArray;
	
	FAbilityDataTable* CurrentAbilityData;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 bIsButtonEventSetup : 1;
	
	UPROPERTY(VisibleDefaultsOnly)
	uint8 bMapGimmickIsExist : 1;

protected:
	UPROPERTY(EditAnywhere)
	uint8 bIsTest : 1;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "bIsTest == true", EditConditionHides))
	uint8 TestAbilityDataArrayNum1;

	UPROPERTY(EditAnywhere, meta=(EditCondition = "bIsTest == true", EditConditionHides))
	uint8 TestAbilityDataArrayNum2;
	
	UPROPERTY(EditAnywhere, meta=(EditCondition = "bIsTest == true", EditConditionHides))
	uint8 TestAbilityDataArrayNum3;
};
