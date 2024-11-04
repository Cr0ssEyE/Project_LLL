// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_PlayerCharacterAttributeSet.generated.h"

USTRUCT(BlueprintType)
struct FPlayerCharacterStatusData
{
	GENERATED_BODY()

public:
	FPlayerCharacterStatusData():
	MaxHealth(0),
	CurrentHealth(0),
	OffencePower(0),
	MoveSpeed(0),
	AttackSpeed(0),
	CriticalChance(0),
	CriticalAmplify(0),
	MaxDashCount(0),
	DashDistance(0),
	KnockBackPower1(0),
	KnockBackPower2(0),
	KnockBackConstant(0.0)
	{
		
	}

public:
	UPROPERTY()
	uint32 MaxHealth;

	UPROPERTY()
	uint32 CurrentHealth;

	UPROPERTY()
	uint32 OffencePower;

	UPROPERTY()
	uint32 MoveSpeed;

	UPROPERTY()
	uint32 AttackSpeed;

	UPROPERTY()
	float CriticalChance;

	UPROPERTY()
	float CriticalAmplify;
	
	UPROPERTY()
	uint32 MaxDashCount;

	UPROPERTY()
	float DashDistance;

	UPROPERTY()
	uint32 KnockBackPower1;

	UPROPERTY()
	uint32 KnockBackPower2;

	UPROPERTY()
	float KnockBackConstant;
};

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

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, EvasionRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashCorrectionDistance);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashCoolDown);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashInvincibleTime);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPower1);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPower2);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackConstant);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FalloutablePower);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackRange);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackRange);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackDamage);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackDamage);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MinChargeAttackKnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackKnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxChargeAttackChargingTime);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxMana);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CurrentMana);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ChargeAttack1ManaCost);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ChargeAttack2ManaCost);
	
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
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MoveSpeedPlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, BaseAttackKnockBackPowerPlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashDistancePlus);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, FasterAttackAttackSpeedRate);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, BleedingExplosionOffencePower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CriticalChancePlus);

public:
	FPlayerCharacterStatusData MakeCharacterStatusData() const;
	void InitializeSavedStatusData(const FPlayerCharacterStatusData* CharacterStatusData);
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// 스테이터스 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData LowHealthPercentage;

	// 회피 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData EvasionRate;
	
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
	FGameplayAttributeData KnockBackPower1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPower2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackConstant;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FalloutablePower;

	// 차지 공격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MinChargeAttackDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxChargeAttackDuration;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentMana;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChargeAttack1ManaCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChargeAttack2ManaCost;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MoveSpeedPlus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BaseAttackKnockBackPowerPlus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashDistancePlus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FasterAttackAttackSpeedRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BleedingExplosionOffencePower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalChancePlus;
};
