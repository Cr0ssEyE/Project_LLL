// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "System/MapGimmick/LLL_MapGimmick.h"
#include "LLL_SaveGameData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	ULLL_SaveGameData();

	UPROPERTY(BlueprintReadWrite, Category = "SaveFileInfo")
	FString SaveFileName;

	UPROPERTY(BlueprintReadWrite, Category = "SaveFileInfo")
	int32 SaveFileIndex;

	// 영구 저장 계통
public:
	UPROPERTY(BlueprintReadWrite, Category = "PermanentSaveInfo")
	float StoryProgress;

	// 임시 영구 보상 변수 
	UPROPERTY(BlueprintReadWrite, Category = "PermanentSaveInfo")
	int32 PermanentRewardAmount;

	// TODO: NPC 개별 스토리나 대화 진척도 있으면 데이터 추가해야함
	
	// 중간 저장 계통(=휘발성)
public:
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FName LastPlayedLevelName;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FStageInfoData StageInfoData;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FPlayerCharacterStatusData PlayerCharacterStatusData;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	TArray<int32> AcquiredEruriasID;

	// ID - Count
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	TMap<int32, int32> AcquiredEruriasStackCount;
	
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 UsedGoldAmount;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 CurrentGoldAmount;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 AcquiredKvassCount;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 AcquiredGoldAppleCount;
	
};
