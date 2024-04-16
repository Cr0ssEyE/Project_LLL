// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLL_DashMonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLL_DashMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_DashMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	ILLL_DashMonsterInterface() : bIsDashing(false) {}
	
	virtual void Dash() = 0;
	
	FORCEINLINE virtual void SetDash(bool IsDashing) { bIsDashing = IsDashing; }
	FORCEINLINE virtual bool IsDashing() { return bIsDashing; }

protected:
	uint8 bIsDashing : 1;
};
