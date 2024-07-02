// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLL_FModInterface.generated.h"

enum class EFModParameter : uint8;
class UFMODAudioComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULLL_FModInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_FModInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UFMODAudioComponent* GetFModAudioComponent() const = 0;
	virtual void SetFModParameter(EFModParameter FModParameter) = 0;
};
