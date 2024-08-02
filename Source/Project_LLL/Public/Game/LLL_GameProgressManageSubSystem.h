// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_LevelNames.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Util/Save/LLL_SaveGameData.h"
#include "LLL_GameProgressManageSubSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLastSessionMapDataLoaded, FStageInfoData, StageInfoData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastSessionPlayerInfoLoaded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveCompleted);

class ULLL_SaveGameData;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GameProgressManageSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULLL_GameProgressManageSubSystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	// 타이틀에서 게임 로드 했을 때 동작, 해당 데이터 기반으로 게임 초기 세팅 지정
	void InitializeGameProgressInfo(ULLL_SaveGameData* SaveGameData = nullptr);

	// 마지막 맵 정보 초기화
	void InitializeSessionMapData() const;
	
	// 게임 진행도 저장
	void SaveGameProgressInfo();

	// 게임 진행도 가져오기
	void LoadGameProgressInfo();

	// 마지막으로 플레이한 세션 정보 가져오기
	void LoadLastSessionMapData();

	void LoadLastSessionPlayerData();
	
public:
	FORCEINLINE void RegisterMapGimmick(ALLL_MapGimmick* MapGimmick) { CurrentInstanceMapGimmick = MapGimmick; }
	FORCEINLINE ULLL_SaveGameData* GetCurrentSaveGameData() const { return CurrentSaveGameData; }
	FORCEINLINE FName GetLastPlayedLevelName() { return CurrentSaveGameData? CurrentSaveGameData->LastPlayedLevelName : LEVEL_LOBBY; }

public:
	FOnLastSessionMapDataLoaded OnLastSessionLoaded;
	FOnLastSessionPlayerInfoLoaded OnLastSessionPlayerDataLoaded;
	FOnSaveCompleted OnSaveCompleted;
	
private:
	void SavePermanentData();
	void SaveLastSessionMapData();
	void SaveLastSessionPlayerData();
	
private:
	UPROPERTY()
	TObjectPtr<ULLL_SaveGameData> CurrentSaveGameData;

	UPROPERTY()
	TWeakObjectPtr<ALLL_MapGimmick> CurrentInstanceMapGimmick;
	
};
