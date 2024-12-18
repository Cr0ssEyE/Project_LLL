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
	virtual void SetDashing(const bool IsDashing) = 0;
	virtual bool IsDashing() const = 0;
	
	virtual float GetMaxDashDistance() const = 0;
	virtual float GetMinDashDistance() const = 0;
	virtual float GetDashSpeed() const = 0;
};
