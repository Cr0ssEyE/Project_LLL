// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "DataTable/LLL_StringDataTable.h"
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


public:
	// 데이터 테이블 Getter
	FORCEINLINE TArray<FAbilityDataTable> GetAbilityDataTable() const { return AbilityData; }
	FORCEINLINE TArray<FFModParameterDataTable> GetFModParameterDataArray() const { return FModParameterData; }
	FORCEINLINE TArray<FRewardDataTable> GetRewardDataTable() const { return RewardData; }
	FORCEINLINE const UDataTable* GetStringDataTable() const { return StringDataTable; }
	FORCEINLINE TArray<FStringDataTable> GetStringDataTablesData() const { return StringData; }

	FORCEINLINE bool CheckCustomTimeDilationIsChanging() const { return bCustomTimeDilationIsChanging; }
	void SetActorsCustomTimeDilationRecursive(TArray<AActor*> Actors, float InCustomTimeDilation);
	
	// 머티리얼 파라미터 컬렉션 
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PlayerMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> ObjectMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> MonsterMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> InterfaceMPC;

	// 데이터 테이블 변수
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> AbilityDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FAbilityDataTable> AbilityData;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const UDataTable> FModParameterDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FFModParameterDataTable> FModParameterData;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> RewardDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FRewardDataTable> RewardData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> StringDataTable;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<FStringDataTable> StringData;
	
protected:
	UPROPERTY(VisibleAnywhere)
	float CustomTimeDilation;

	UPROPERTY(EditAnywhere)
	float CustomTimeDilationInterpSpeed;

	UPROPERTY(VisibleAnywhere)
	uint8 bCustomTimeDilationIsChanging : 1;
};
