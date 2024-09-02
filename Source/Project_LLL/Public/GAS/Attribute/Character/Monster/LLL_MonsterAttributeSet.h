// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_MonsterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()
	
public:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MaxShield);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, CurrentShield);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, AttackDistance);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, DetectDistance);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, ProximityDetectDistance);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, ChargeTimer);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, AttackCoolDown);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, FieldOfView);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, FindPatrolPosRadius);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, DestroyTimer);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, ClusterRecognizeRadius);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData1);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData2);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData3);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData4);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData5);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData6);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData7);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData8);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData9);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData10);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData11);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData12);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData13);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, MonsterData14);

protected:
	virtual void CheckAbnormalStatus(const FGameplayEffectModCallbackData& Data);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentShield;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DetectDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ProximityDetectDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChargeTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackCoolDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FieldOfView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FindPatrolPosRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DestroyTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ClusterRecognizeRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData4;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData6;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData7;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData8;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData9;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData10;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData11;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData12;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData13;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MonsterData14;
};
