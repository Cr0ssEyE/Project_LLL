// Fill out your copyright notice in the Description page of Project Settings.

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

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxAttackAction);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, AttackActionIntervalTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, AttackActionInputDelayTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, DashInvincibleTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, RushSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackPower);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CurrentComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MultiplyComboCountWhenHit);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MultiplyComboCountPerTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxComboStackDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CurrentSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, SkillGaugeAmplifyByCombo);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, SkillGaugeAmplifyByItem);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, bIsComboTimerElapsed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, AddCurrentSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, BulletTimeDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, BulletTimeWorldDecelerationRate);
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	void TryStartComboManagement(const FGameplayEffectModCallbackData& Data);
	
	// 일반 공격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxAttackAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackActionIntervalTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackActionInputDelayTime;

	// 이동 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxDashCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashInvincibleTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData RushSpeed;

	// 넉백 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPower;
	
	// 콤보 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentComboCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxComboCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MultiplyComboCountWhenHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MultiplyComboCountPerTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxComboStackDuration;

	// 스킬 게이지 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByItem;

	// 스킬 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BulletTimeDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BulletTimeWorldDecelerationRate;
	
	// 메타 어트리뷰트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData bIsComboTimerElapsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddCurrentSkillGauge;
	
};
