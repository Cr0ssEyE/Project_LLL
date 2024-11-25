// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LLL_AbnormalStatusAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AbnormalStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_AbnormalStatusAttributeSet();

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusDamage);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusPeriod);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusDuration);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MaxBleedingStatusCount);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MaxBlowStackCount);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, OffensePowerPerBlowStack);
	
	// 출혈
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusDamage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusPeriod;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxBleedingStatusCount;

	// 일격
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxBlowStackCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData OffensePowerPerBlowStack;
	
};
