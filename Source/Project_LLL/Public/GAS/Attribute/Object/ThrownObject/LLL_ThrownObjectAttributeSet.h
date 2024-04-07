// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Attribute/Object/LLL_ObjectAttributeSetBase.h"
#include "LLL_ThrownObjectAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_ThrownObjectAttributeSet : public ULLL_ObjectAttributeSetBase
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_ThrownObjectAttributeSet, OffensePower);
	ATTRIBUTE_ACCESSORS(ULLL_ThrownObjectAttributeSet, ThrowSpeed);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData OffensePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ThrowSpeed;
};
