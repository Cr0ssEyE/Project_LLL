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

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MarkStatusDuration);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MarkStatusDamageAmplifyPerStack);
	
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDuration);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDamageAmplifyByNormal);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDamageAmplifyByBoss);

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusDuration);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusPeriod);
	
	// 표식
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MarkStatusDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MarkStatusDamageAmplifyPerStack;
	
	// 표적
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDamageAmplifyByNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDamageAmplifyByBoss;
	
	// 출혈
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusPeriod;
	
};
