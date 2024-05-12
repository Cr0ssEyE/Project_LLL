// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Object/Skill/Base/LLL_SkillObjectAttributeSet.h"
#include "LLL_PhysicalProofAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PhysicalProofAttributeSet : public ULLL_SkillObjectAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_PhysicalProofAttributeSet, PushPower);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
    FGameplayAttributeData PushPower;
};
