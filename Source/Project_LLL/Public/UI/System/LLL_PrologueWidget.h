// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PrologueWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PrologueWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void OpenStage1Level();

};
