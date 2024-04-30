// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_TestAbilityDataTable.h"
#include "DataTable/LLL_TestShopDataTable.h"
#include "LLL_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_LLL_API ULLL_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	ULLL_GameInstance();

	virtual void Init() override;

	FORCEINLINE TArray<FFModParameterDataTable> GetFModParameterDataArray() const { return FModParameterDataArray; }

public:
	// 데이터 테이블 Getter
	FORCEINLINE TArray<FTestAbilityDataTable> GetAbilityDataTable() const { return AbilityData; }
	FORCEINLINE TArray<FTestRewardDataTable> GetRewardDataTable() const { return RewardData; }
	FORCEINLINE TArray<FTestShopDataTable> GetShopDataTable() const { return ShopData; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PlayerMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> ObjectMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> MonsterMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> InterfaceMPC;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const UDataTable> FModParameterDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FFModParameterDataTable> FModParameterDataArray;

	// 데이터 테이블 변수
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> AbilityDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestAbilityDataTable> AbilityData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> RewardDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestRewardDataTable> RewardData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> ShopDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestShopDataTable> ShopData;
	
};
