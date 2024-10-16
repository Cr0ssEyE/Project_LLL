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

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MaxMarkStack);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MarkStatusDamageAmplifyPerStack);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, MarkStatusDuration);

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDamageAmplifyByNormal);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDamageAmplifyByBoss);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, TargetingStatusDuration);

	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusPeriod);
	ATTRIBUTE_ACCESSORS(ULLL_AbnormalStatusAttributeSet, BleedingStatusDuration);
	
	// 표식
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMarkStack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MarkStatusDamageAmplifyPerStack;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MarkStatusDuration;
	
	// 표적
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDamageAmplifyByNormal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDamageAmplifyByBoss;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingStatusDuration;
	
	// 출혈
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusPeriod;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingStatusDuration;
	
};
