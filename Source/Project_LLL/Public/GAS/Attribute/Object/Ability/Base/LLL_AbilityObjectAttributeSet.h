// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/Object/Base/LLL_ObjectAttributeSetBase.h"
#include "LLL_AbilityObjectAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AbilityObjectAttributeSet : public ULLL_ObjectAttributeSetBase
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_AbilityObjectAttributeSet, OffensePower);
	ATTRIBUTE_ACCESSORS(ULLL_AbilityObjectAttributeSet, DestroyTimer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData OffensePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DestroyTimer;
};
