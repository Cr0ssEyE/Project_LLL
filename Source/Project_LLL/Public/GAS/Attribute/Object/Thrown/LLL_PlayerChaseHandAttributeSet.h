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

	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, ThrowSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, MinimumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, MaximumThrowDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, CorrectionReachStateDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, ReleaseSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, ChaseSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, GrabCollisionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerChaseHandAttributeSet, GrabDuration);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ThrowSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinimumThrowDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaximumThrowDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CorrectionReachStateDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ReleaseSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChaseSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData GrabCollisionRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData GrabDuration;
};
