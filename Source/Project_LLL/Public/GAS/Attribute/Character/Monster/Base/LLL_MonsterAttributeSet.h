// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_MonsterAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadAttributeDataDelegate, float, Value);

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, AttackDistance);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, DetectDistance);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, FieldOfView);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, FindPatrolPosRadius);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, DestroyTimer);

	FOnLoadAttributeDataDelegate OnLoadMaxHealthDelegate;

protected:
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DetectDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FindPatrolPosRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DestroyTimer;
};
