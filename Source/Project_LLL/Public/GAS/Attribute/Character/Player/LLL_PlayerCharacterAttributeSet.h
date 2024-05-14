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
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackDamage);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, KnockBackInfluence);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ChaseActionDamageAmplify);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ChaseCoolDown);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ChaseReduceCoolDownPerHit);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, CurrentComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxComboCount);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MultiplyComboCountWhenHit);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MultiplyComboCountPerTime);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, MaxComboKeepingDuration);
	
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, bIsComboTimerElapsed);

	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, TargetingCorrectionRadius);
	ATTRIBUTE_ACCESSORS(ULLL_PlayerCharacterAttributeSet, ImpulseStrength);
	
protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	void TryStartComboManagement(const FGameplayEffectModCallbackData& Data);
	
	// 일반 공격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxAttackActionCount;

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

	// 추격 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChaseActionDamageAmplify;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChaseCoolDown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ChaseReduceCoolDownPerHit;
	
	// 넉백 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData KnockBackInfluence;
	
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
	FGameplayAttributeData MaxComboKeepingDuration;
	
	// 기타 효과 관련
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData TargetingCorrectionRadius;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ImpulseStrength;
	
	// 메타 어트리뷰트
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData bIsComboTimerElapsed;

};
