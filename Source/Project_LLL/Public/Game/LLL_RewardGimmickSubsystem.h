// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "LLL_RewardGimmickSubsystem.generated.h"

class UAbilitySystemComponent;
class ULLL_ExtendedGameplayEffect;
class ALLL_GateObject;
struct FRewardDataTable;
struct FAbilityDataTable;
class ULLL_RewardDataTable;
class ULLL_TestAbilityDataTable;
class ALLL_AbilityRewardObject;

USTRUCT(BlueprintType)
struct FTestAbilityDataID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TestAbilityDataID1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TestAbilityDataID2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TestAbilityDataID3;
};

UCLASS()
class PROJECT_LLL_API ULLL_RewardGimmickSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ULLL_RewardGimmickSubsystem();

	FORCEINLINE const FRewardDataTable* GetRewardData(uint8 index) { return RewardData[index]; }
	FORCEINLINE void InformMapGimmickIsExist() { bMapGimmickIsExist = true; }
	FORCEINLINE TArray<const FAbilityDataTable*> GetRolledAbilityData() const { return ButtonAbilityDataArray; }

public:
	UFUNCTION()
	void SetRewardToGate(ALLL_GateObject* Gate);
	
	UFUNCTION()
	void SetRewardButtons();

	void SetDataTable();

	void SetRewardWeight();

	void RollReward(TArray<TTuple<const FAbilityDataTable*, float>> AbilityDataTables);
	
	bool IsTest() const;

	void PlayerInitialize();
	
protected:
	void WaitPlayerInitialize();
	
	UFUNCTION()
	void ClickFirstButton();

	UFUNCTION()
	void ClickSecondButton();

	UFUNCTION()
	void ClickThirdButton();
	
	void ClickButtonEvent(const FAbilityDataTable* ButtonAbilityData);

	UFUNCTION()
	void ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID);
	
protected:
	TArray<const FRewardDataTable*> RewardData;
	TArray<const FAbilityDataTable*> AbilityData;

	uint32 TotalRewardWeight;
	TArray<TTuple<const FAbilityDataTable*, float>> NormalizedWeightRewardArray;
	
	TArray<const FAbilityDataTable*> ButtonAbilityDataArray;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 bIsButtonEventSetup : 1;
	
	UPROPERTY(VisibleDefaultsOnly)
	uint8 bMapGimmickIsExist : 1;
	
	const FAbilityDataTable* ButtonAbilityData1;
	const FAbilityDataTable* ButtonAbilityData2;
	const FAbilityDataTable* ButtonAbilityData3;

	// 테스트용
public:
	UPROPERTY(EditAnywhere)
	TArray<FTestAbilityDataID> TestAbilityDataID;
};
