// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceActor.h"
#include "LLL_LevelSequenceManager.generated.h"

enum class ELevelSequenceType : uint8;
class ULLL_GlobalLevelSequenceDataAsset;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_LevelSequenceManager : public ALevelSequenceActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnEncounteredCallBack(ELevelSequenceType SequenceType);

	UFUNCTION()
	void OnFinishedCallBack();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<ULLL_GlobalLevelSequenceDataAsset> LevelSequenceDataAsset;
};
