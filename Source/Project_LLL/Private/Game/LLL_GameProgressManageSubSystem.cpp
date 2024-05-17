// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameProgressManageSubSystem.h"

#include "Constant/LLL_GameplayInfo.h"
#include "Kismet/GameplayStatics.h"

ULLL_GameProgressManageSubSystem::ULLL_GameProgressManageSubSystem()
{
	
}

void ULLL_GameProgressManageSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (ULLL_SaveGameData* SaveData = Cast<ULLL_SaveGameData>(UGameplayStatics::LoadGameFromSlot(DEFAULT_FILE_NAME, DEFAULT_FILE_INDEX)))
	{
		CurrentSaveGameData = SaveData;
	}
}

void ULLL_GameProgressManageSubSystem::Deinitialize()
{
	if (IsValid(CurrentSaveGameData))
	{
		UGameplayStatics::SaveGameToSlot(CurrentSaveGameData, CurrentSaveGameData->SaveFileName, CurrentSaveGameData->SaveFileIndex);
	}
	
	Super::Deinitialize();
}

void ULLL_GameProgressManageSubSystem::InitializeGameProgressInfo(ULLL_SaveGameData* SaveGameData)
{
	ULLL_SaveGameData* SaveData = SaveGameData;
	if (SaveData == nullptr)
	{
		SaveData = GetCurrentSaveGameData();
	}

	if (SaveData != GetCurrentSaveGameData())
	{
		CurrentSaveGameData = SaveData;
	}

	// Do Something
}
