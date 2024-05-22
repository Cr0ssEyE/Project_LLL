// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "LLL_DropGoldInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLL_DropGoldInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_DropGoldInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DropGold(const FGameplayTag tag, int32 data) = 0;
};
