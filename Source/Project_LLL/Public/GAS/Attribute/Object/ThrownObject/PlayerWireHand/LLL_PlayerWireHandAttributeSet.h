// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GAS/Attribute/Object/ThrownObject/Base/LLL_ThrownObjectAttributeSet.h"
#include "LLL_PlayerWireHandAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerWireHandAttributeSet : public ULLL_ThrownObjectAttributeSet
{
	GENERATED_BODY()

public:
	ULLL_PlayerWireHandAttributeSet();

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, MinimumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, MaximumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, CorrectionReachStateDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, ReleaseSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, CollisionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerWireHandAttributeSet, GrabDuration);
	
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
