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
	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "타이틀")
	TObjectPtr<UFMODEvent> BGMTitle;

	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "1스테이지")
	TObjectPtr<UFMODEvent> BGMStage1;
	
	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "1스테이지 보스")
	TObjectPtr<UFMODEvent> BGMStage1Boss;
	
	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "상점")
	TObjectPtr<UFMODEvent> BGMShop;
	
	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "사슴")
	TObjectPtr<UFMODEvent> BGMDeer;

	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "늑대")
	TObjectPtr<UFMODEvent> BGMWolf;
	
	UPROPERTY(EditDefaultsOnly, Category = "BGM", DisplayName = "까마귀")
	TObjectPtr<UFMODEvent> BGMRaven;

	UPROPERTY(EditDefaultsOnly, Category = "FMod", DisplayName = "환경음")
	TObjectPtr<UFMODEvent> AMB;
};
