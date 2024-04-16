// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/LLL_MeleeMonsterAttributeSet.h"
#include "LLL_SwordDashAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SwordDashAttributeSet : public ULLL_MeleeMonsterAttributeSet
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_SwordDashAttributeSet, MinDashDistance);
	ATTRIBUTE_ACCESSORS(ULLL_SwordDashAttributeSet, MaxDashDistance);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinDashDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxDashDistance;
};
