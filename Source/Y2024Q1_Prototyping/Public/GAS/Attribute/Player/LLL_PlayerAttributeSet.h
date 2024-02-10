// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	ULLL_PlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, CurrentDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, DashSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, DashInvincibleTime);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentDashCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxDashCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashInvincibleTime;
	
};
