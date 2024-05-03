// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"
#include "LLL_PlayerChaseHandAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerChaseHandAttributeSet : public ULLL_ThrownObjectAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_PlayerChaseHandAttributeSet();

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, MinimumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, MaximumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, CorrectionReachStateDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, ReleaseSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, CollisionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, GrabDuration);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinimumThrowDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaximumThrowDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CorrectionReachStateDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ReleaseSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CollisionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData GrabDuration;
};
