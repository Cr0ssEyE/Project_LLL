// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Attribute/Base/LLL_CharacterAttributeSetBase.h"
#include "LLL_PlayerAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerAttributeSet : public ULLL_CharacterAttributeSetBase
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
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, CurrentComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, MaxComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerAttributeSet, ComboStackDuration);
	
	// 일반 공격 관련
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxAttackAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackActionIntervalTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData AttackActionInputDelayTime;

	// 대쉬 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxDashCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData DashInvincibleTime;

	// 콤보 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CurrentComboCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxComboCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ComboStackDuration;

	
};
