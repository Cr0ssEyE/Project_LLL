// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_AbilityDataTable.h"
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
	FORCEINLINE bool CheckCustomTimeDilationIsChanging() const { return bCustomTimeDilationIsChanging; }

public:
	// 데이터 테이블 Getter
	FORCEINLINE TArray<FAbilityDataTable> GetAbilityDataTable() const { return AbilityData; }
	FORCEINLINE TArray<FRewardDataTable> GetRewardDataTable() const { return RewardData; }

public:
	void SetActorsCustomTimeDilation(const TArray<AActor*>& Actors, float InCustomTimeDilation);

protected:
	void SetActorsCustomTimeDilationRecursive(TArray<AActor*> Actors, float InCustomTimeDilation);

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
	TArray<FAbilityDataTable> AbilityData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> RewardDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FRewardDataTable> RewardData;

	UPROPERTY(VisibleAnywhere)
	float CustomTimeDilation;

	UPROPERTY(EditAnywhere)
	float CustomTimeDilationInterpSpeed;

	UPROPERTY(VisibleAnywhere)
	uint8 bCustomTimeDilationIsChanging : 1;
};
