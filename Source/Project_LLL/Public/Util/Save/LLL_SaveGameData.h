// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "System/MapGimmick/LLL_MapGimmick.h"
#include "LLL_SaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerPlayProgressData
{
	GENERATED_BODY()

public:
	FPlayerPlayProgressData() :
	UsedGoldAmount(0),
	CurrentGoldAmount(0),
	AcquiredKvassCount(0),
	AcquiredGoldAppleCount(0)
	{
		
	}

public:
	// Index - ID.
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	TMap<int32, int32> ShoppingProductList;
	
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	TArray<int32> AcquiredEruriasID;

	// ID - Count
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	TMap<int32, int32> AcquiredEruriasStackCount;

	// 사용 골드. 
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 UsedGoldAmount;

	// 현재 보유중인 골드.
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 CurrentGoldAmount;

	// 크바스(=강화) 획득 횟수
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 AcquiredKvassCount;

	// 황금사과(=최대체력) 획득 횟수
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	int32 AcquiredGoldAppleCount;
};

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

	// TODO: NPC 개별 스토리나 대화 진척도 있으면 데이터 추가해야함. 구조체로 묶기?
	
	// 중간 저장 계통(=휘발성)
public:
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FName LastPlayedLevelName;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FStageInfoData StageInfoData;
	
	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FPlayerCharacterStatusData PlayerCharacterStatusData;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FPlayerPlayProgressData PlayerPlayProgressData;

	UPROPERTY(BlueprintReadWrite, Category = "LastSessionSaveInfo")
	FVector LobbyLastStandingLocation;

};
