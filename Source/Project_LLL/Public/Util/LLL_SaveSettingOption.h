// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LLL_SaveSettingOption.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SaveSettingOption : public USaveGame
{
	GENERATED_BODY()
public:
	ULLL_SaveSettingOption();

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	FString SaveFileName;
};
