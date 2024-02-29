// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LLLSaveSettingOption.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLLSaveSettingOption : public USaveGame
{
	GENERATED_BODY()
public:
	ULLLSaveSettingOption();

	UPROPERTY(EditDefaultsOnly, Category = SettingOption)
	FString SaveFileName;
};
