// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "LLL_EncounterLevelSequenceActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_EncounterLevelSequenceActor : public ALevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnEncountedCallBack();

	UFUNCTION()
	void OnFinishedCallBack();
	
};
