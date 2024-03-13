// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LLL_CharacterWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CharacterWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
