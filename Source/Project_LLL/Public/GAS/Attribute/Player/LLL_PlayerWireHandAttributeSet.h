// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LLL_PlayerWireHandAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerWireHandAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_PlayerWireHandAttributeSet();

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, ThrowSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, CorrectionReachStateDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, ReleaseSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, CollisionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, GrabDuration);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ThrowSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CorrectionReachStateDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ReleaseSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CollisionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData GrabDuration;
};
