// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "LLL_BaseCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetDataAsset(const ULLL_BaseCharacterDataAsset* InCharacterDataAsset) { CharacterDataAsset = InCharacterDataAsset; }

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BaseCharacterDataAsset> CharacterDataAsset;
};
