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
	ATTRIBUTE_ACCESSORS(ULLL_SwordDashAttributeSet, DashDistance);
	ATTRIBUTE_ACCESSORS(ULLL_SwordDashAttributeSet, DashDamage);
	ATTRIBUTE_ACCESSORS(ULLL_SwordDashAttributeSet, DashDamageRange);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashDamageRange;
};
