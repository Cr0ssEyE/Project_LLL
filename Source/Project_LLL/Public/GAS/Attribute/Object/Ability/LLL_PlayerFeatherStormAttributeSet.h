// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Object/Ability/Base/LLL_AbilityObjectAttributeSet.h"
#include "LLL_PlayerFeatherStormAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerFeatherStormAttributeSet : public ULLL_AbilityObjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_PlayerFeatherStormAttributeSet, DamageTimer);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DamageTimer;
};
