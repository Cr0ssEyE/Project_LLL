// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Constant/LLL_LevelNames.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Util/Save/LLL_SaveGameData.h"
#include "LLL_GameProgressManageSubSystem.generated.h"

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

	// 타이틀에서 게임 로드 했을 때 동작, 해당 데이터 기반으로 게임 초기 세팅 지정
	void InitializeGameProgressInfo(ULLL_SaveGameData* SaveGameData = nullptr);

	FORCEINLINE ULLL_SaveGameData* GetCurrentSaveGameData() { return CurrentSaveGameData; }
	
public:
	FORCEINLINE FName GetLastPlayedLevelName() { return CurrentSaveGameData? CurrentSaveGameData->LastPlayLevelName : LEVEL_INTRO; }
	
private:
	UPROPERTY()
	TObjectPtr<ULLL_SaveGameData> CurrentSaveGameData;

};
