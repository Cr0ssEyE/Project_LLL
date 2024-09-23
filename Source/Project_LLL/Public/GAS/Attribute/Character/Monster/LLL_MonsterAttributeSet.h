// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_MonsterAttributeSet.generated.h"

#define GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(MonsterName, DataName, PropertyName) \
	FORCEINLINE float Get##MonsterName##DataName() const \
	{ \
		return PropertyName.GetCurrentValue(); \
	}

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()
	
public:
	ULLL_MonsterAttributeSet();
	
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
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, Weight);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, FindPatrolPosRadius);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, DestroyTimer);
	ATTRIBUTE_ACCESSORS(ULLL_MonsterAttributeSet, ClusterRecognizeRadius);

	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ClawBasic, ThrowSpeed, MonsterData1);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ClawBasic, PredictionRate, MonsterData2);
	
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(SwordDash, MaxDashDistance, MonsterData1);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(SwordDash, MinDashDistance, MonsterData2);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(SwordDash, DashDamageRange, MonsterData4);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(SwordDash, DashSpeed, MonsterData6);
	
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, DashDistance, MonsterData1);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, DashSpeed, MonsterData2);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, DashDamageRange, MonsterData3);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, ShockwaveChargeTimer, MonsterData4);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, AttackInApneaDashDistance, MonsterData5);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, AttackInApneaDashSpeed, MonsterData7);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, AttackInApneaAttackCount, MonsterData8);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, AttackInApneaChargeTimer, MonsterData10);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, ThrowSpeed, MonsterData11);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, OtherMonsterDetectRange, MonsterData12);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, PredictionRate, MonsterData13);
	GAMEPLAYATTRIBUTE_CUSTOM_VALUE_GETTER(ManOfStrength, ThrowOffencePower, MonsterData14);

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
	FGameplayAttributeData Weight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FindPatrolPosRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DestroyTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ClusterRecognizeRadius;

protected:
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
