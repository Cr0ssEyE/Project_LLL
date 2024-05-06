// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LLL_PlayerSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_PlayerSkillAttributeSet();
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, CurrentSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, MaxSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, SkillGaugeAmplifyByCombo);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, SkillGaugeAmplifyByItem);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, AddCurrentSkillGauge);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, BulletTimeDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerSkillAttributeSet, BulletTimeWorldDecelerationRate);
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	// 스킬 게이지 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ConsumeSkillGaugeAmount;
	
	// 스킬 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BulletTimeDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BulletTimeWorldDecelerationRate;

	// 메타 어트리뷰트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddCurrentSkillGauge;
};
