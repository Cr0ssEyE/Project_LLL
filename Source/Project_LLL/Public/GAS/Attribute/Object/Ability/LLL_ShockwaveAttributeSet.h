// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Object/Ability/Base/LLL_AbilityObjectAttributeSet.h"
#include "LLL_ShockwaveAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ShockwaveAttributeSet : public ULLL_AbilityObjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_ShockwaveAttributeSet, Speed);
	ATTRIBUTE_ACCESSORS(ULLL_ShockwaveAttributeSet, Thickness);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Thickness;
};
