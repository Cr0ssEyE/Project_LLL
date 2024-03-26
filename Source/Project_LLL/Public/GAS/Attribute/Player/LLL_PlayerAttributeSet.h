// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerAttributeSet : public ULLL_CharacterAttributeSetBase
{
	GENERATED_BODY()

public:
	ULLL_PlayerAttributeSet();

	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxAttackAction);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, AttackActionIntervalTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, AttackActionInputDelayTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxDashCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, DashSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, DashInvincibleTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, RushSpeed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, CurrentComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MultiplyComboCountWhenHit);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MultiplyComboCountPerTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxComboStackDuration);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, CurrentSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxSkillGauge);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, SkillGaugeAmplifyByCombo);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, SkillGaugeAmplifyByItem);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, bIsComboTimerElapsed);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, AddCurrentSkillGauge);
	
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

	// 스킬 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxSkillGauge;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData SkillGaugeAmplifyByItem;
	
	// 메타 어트리뷰트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData bIsComboTimerElapsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AddCurrentSkillGauge;
	
};
