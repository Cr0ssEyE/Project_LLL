// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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

	UPROPERTY(BlueprintReadWrite, Category = SettingOption)
	FString SaveFileName;

	UPROPERTY(BlueprintReadWrite, Category = SettingOption)
	int32 SaveFileIndex;

public:
	UPROPERTY(BlueprintReadWrite, Category = SettingOption)
	float StoryProgress;

	UPROPERTY(BlueprintReadWrite, Category = SettingOption)
	FName LastPlayLevelName;
};
