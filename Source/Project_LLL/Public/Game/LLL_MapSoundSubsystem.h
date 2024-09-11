// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODBlueprintStatics.h"
#include "DataAsset/LLL_MapSoundSubsystemDataAsset.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "LLL_MapSoundSubsystem.generated.h"

class ALLL_PlayerBase;
class UBoxComponent;
class UFMODEvent;
class ALLL_BaseCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MapSoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULLL_MapSoundSubsystem();

	FORCEINLINE void SetFModParameterDataArray(const TArray<FFModParameterDataTable>& InFModParameterDataArray) { FModParameterDataArray = InFModParameterDataArray; }
	
	void SetBulletTimeParameterValue(float Value) const;
	void SetBattleParameter(float Value) const;
	void SetPauseParameter(float Value) const;
	void PlayBGM();
	void PlayAMB();
	void StopBGM() const;
	void StopAMB() const;
	void PlayerDeadEvent() const;
	
protected:
	UPROPERTY()
	TObjectPtr<const ULLL_MapSoundSubsystemDataAsset> MapSoundSubsystemDataAsset;

	FFMODEventInstance BGMWrapper;
	FFMODEventInstance AMBWrapper;
	
	TArray<FFModParameterDataTable> FModParameterDataArray;
};
