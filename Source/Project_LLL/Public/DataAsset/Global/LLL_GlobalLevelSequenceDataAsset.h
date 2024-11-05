// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_GlobalLevelSequenceDataAsset.generated.h"

enum class ELevelSequenceType : uint8;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GlobalLevelSequenceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "StageOne")
	TMap<ELevelSequenceType, TObjectPtr<ULevelSequence>> Sequences;
	
};
