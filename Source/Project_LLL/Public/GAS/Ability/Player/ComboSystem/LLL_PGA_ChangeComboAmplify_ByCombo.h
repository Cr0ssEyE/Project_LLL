// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ChangeComboAmplify_ByCombo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ChangeComboAmplify_ByCombo : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ChangeComboAmplify_ByCombo();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void ApplySkillGaugeAmplify();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName ="스킬 게이지 콤보 배율 조정 이펙트")
	TSubclassOf<UGameplayEffect> UpdateComboAmplifyEffect;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName ="커브 테이블 내 콤보 비례 스킬 게이지 배율 변화값 행")
	FCurveTableRowHandle ComboAmplifyChangeSection;
	
	UPROPERTY()
	uint32 CurrentComboAmplifyLevel;

	UPROPERTY()
	uint32 CurrentAmplifyLevelComboCount;
};
