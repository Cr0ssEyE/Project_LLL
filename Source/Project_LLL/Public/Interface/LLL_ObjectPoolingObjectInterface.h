// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLL_ObjectPoolingObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLL_ObjectPoolingObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_ObjectPoolingObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FORCEINLINE bool IsActivated() const { return bIsActivated; }
	
	virtual void Activate() { bIsActivated = true; }
	virtual void Deactivate() { bIsActivated = false; }

protected:
	uint8 bIsActivated : 1 = false;
};
