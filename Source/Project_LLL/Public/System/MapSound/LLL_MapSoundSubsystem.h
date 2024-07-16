// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODBlueprintStatics.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_MapSoundManager.generated.h"

class UBoxComponent;
class UFMODEvent;
class ALLL_BaseCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MapSoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ALLL_MapSoundManager();
	
	void SetPitch(float InPitch) const;
	void SetBattleParameter(float Value) const;
	void SetPauseParameter(float Value) const;
	void StopBGM() const;
	
protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	
	UFUNCTION()
	void PlayerDeadHandle(ALLL_BaseCharacter* Character);

	UPROPERTY(EditAnywhere, Category = "FMod", DisplayName = "배경음")
	TObjectPtr<UFMODEvent> BGM;

	UPROPERTY(EditAnywhere, Category = "FMod", DisplayName = "환경음")
	TObjectPtr<UFMODEvent> AMB;

	UPROPERTY(EditAnywhere, Category = "FMod", DisplayName = "배경음 재생")
	uint8 PlayBGM : 1;

	FFMODEventInstance BGMWrapper;
	FFMODEventInstance AMBWrapper;
};
