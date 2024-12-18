// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LLL_NiagaraInterface.generated.h"

class UNiagaraComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class ULLL_NiagaraInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_LLL_API ILLL_NiagaraInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void AddNiagaraComponent(UNiagaraComponent* InNiagaraComponent) = 0;

	virtual TArray<UNiagaraComponent*> GetNiagaraComponents() const = 0;
};
