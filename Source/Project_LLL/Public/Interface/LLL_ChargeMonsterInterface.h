// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLL_ChargeMonsterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLL_ChargeMonsterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_ChargeMonsterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FORCEINLINE virtual void SetCharge(bool IsCharging) { bIsCharging = IsCharging; }
	FORCEINLINE virtual bool IsCharging() const { return bIsCharging; }
	
	virtual void Charge() const = 0;

	uint8 bIsCharging : 1;
};
