// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_AbilityObjectAttributeSet.h"
#include "LLL_ThreateningDeathAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ThreateningDeathAttributeSet : public ULLL_AbilityObjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_ThreateningDeathAttributeSet, PushPower);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData PushPower;
};
