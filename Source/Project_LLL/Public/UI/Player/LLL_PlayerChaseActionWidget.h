// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_PlayerChaseActionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerChaseActionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE void SetPercent(float value) { Percent = value; } 
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = "Variable")
	float Percent;
};
