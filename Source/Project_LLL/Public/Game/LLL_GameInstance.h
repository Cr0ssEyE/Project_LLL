// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "DataTable/LLL_StringDataTable.h"
#include "LLL_GameInstance.generated.h"

class ULLL_RewardGimmickSubsystem;
enum class ELevelSequenceType : uint8;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStageEncounteredDelegate, ELevelSequenceType, SequenceType);

class ULLL_GlobalNiagaraDataAsset;
class ULLL_GlobalParameterDataAsset;
class ULLL_MapSoundSubsystem;

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
	virtual void InitDataTables();
	
	FStageEncounteredDelegate EncounteredDelegate;
	
public:
	// 데이터 테이블 Getter
	FORCEINLINE TArray<const FAbilityDataTable*> GetAbilityDataTable() const { return AbilityData; }
	FORCEINLINE TArray<FFModParameterDataTable> GetFModParameterDataArray() const { return FModParameterData; }
	FORCEINLINE TArray<const FRewardDataTable*> GetRewardDataTable() const { return RewardData; }
	FORCEINLINE const UDataTable* GetStringDataTable() const { return StringDataTable; }
	FORCEINLINE TArray<const FStringDataTable*> GetStringDataTablesData() const { return StringData; }

	// MPC Getter
	FORCEINLINE UMaterialParameterCollection* GetPlayerMPC() const { return PlayerMPC; }
	FORCEINLINE UMaterialParameterCollection* GetObjectMPC() const { return ObjectMPC; }
	FORCEINLINE UMaterialParameterCollection* GetMonsterMPC() const { return MonsterMPC; }
	FORCEINLINE UMaterialParameterCollection* GetPostProcessMPC() const { return PostProcessMPC; }

	// 데이터 에셋
public:
	FORCEINLINE TObjectPtr<const ULLL_GlobalNiagaraDataAsset> GetGlobalNiagaraDataAsset() const { return GlobalNiagaraDataAsset; }
	FORCEINLINE TObjectPtr<const ULLL_GlobalParameterDataAsset> GetGlobalParametersDataAsset() const { return GlobalParametersDataAsset; }
	
	// 머티리얼 파라미터 컬렉션 
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PlayerMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> ObjectMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> MonsterMPC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMaterialParameterCollection> PostProcessMPC;

	// 범용 데이터 에셋
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_GlobalNiagaraDataAsset> GlobalNiagaraDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_GlobalParameterDataAsset> GlobalParametersDataAsset;
	
	// 데이터 테이블 변수
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> AbilityDataTable;
	
	TArray<const FAbilityDataTable*> AbilityData;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const UDataTable> FModParameterDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FFModParameterDataTable> FModParameterData;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> RewardDataTable;
	
	TArray<const FRewardDataTable*> RewardData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> StringDataTable;

	TArray<const FStringDataTable*> StringData;
};
