// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_MapSoundSubsystemDataAsset.generated.h"

class UFMODEvent;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MapSoundSubsystemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "배경음")
	TObjectPtr<UFMODEvent> BGM;

	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "환경음")
	TObjectPtr<UFMODEvent> AMB;
};
