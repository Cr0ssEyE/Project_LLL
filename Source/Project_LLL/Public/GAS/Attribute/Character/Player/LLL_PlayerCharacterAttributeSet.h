﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_PlayerCharacterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerCharacterAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	ULLL_PlayerCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, LowHealthPercentage);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxAttackActionCount);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CriticalChance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CriticalAmplify);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashCorrectionDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashCoolDown);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashInvincibleTime);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackOffencePower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FalloutablePower);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackRange);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackRange);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackDamage);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackDamage);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackKnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackKnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackChargingTime);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FeatherMoveSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, TargetingCorrectionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ImpulseStrength);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, AllOffencePowerRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, AllOffencePowerPlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPowerRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPowerPlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackOffencePowerRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackOffencePowerPlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FeatherOffencePowerRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FeatherOffencePowerPlus);
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 스테이터스 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData LowHealthPercentage;
	
	// 일반 공격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxAttackActionCount;

	// 크리티컬 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalChance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalAmplify;
	
	// 이동 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxDashCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashCorrectionDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashCoolDown;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashInvincibleTime;
	
	// 넉백 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPower;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackOffencePower;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FalloutablePower;

	// 차지 공격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinChargeAttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxChargeAttackRange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinChargeAttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxChargeAttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinChargeAttackKnockBackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxChargeAttackKnockBackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxChargeAttackChargingTime;
	
	// 기타 효과 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FeatherMoveSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingCorrectionRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ImpulseStrength;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AllOffencePowerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AllOffencePowerPlus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPowerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPowerPlus;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackOffencePowerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackOffencePowerPlus;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FeatherOffencePowerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FeatherOffencePowerPlus;
};
