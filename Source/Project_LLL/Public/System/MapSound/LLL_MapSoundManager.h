// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODBlueprintStatics.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_MapSoundManager.generated.h"

class UFMODEvent;
class ALLL_BaseCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MapSoundManager : public ALLL_SystemBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void PlayerDeadHandle(ALLL_BaseCharacter* Character);

	UPROPERTY(EditAnywhere, Category = "FMod", DisplayName = "배경음")
	TObjectPtr<UFMODEvent> BGM;

	UPROPERTY(EditAnywhere, Category = "FMod", DisplayName = "환경음")
	TObjectPtr<UFMODEvent> AMB;

	FFMODEventInstance BGMWrapper;
	FFMODEventInstance AMBWrapper;
};
