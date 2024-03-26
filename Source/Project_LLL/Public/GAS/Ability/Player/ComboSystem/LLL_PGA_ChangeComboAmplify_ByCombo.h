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
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> UpdateComboAmplifyEffect;

	UPROPERTY(EditDefaultsOnly)
	FCurveTableRowHandle ComboAmplifyChangeSection;
	
	UPROPERTY()
	uint32 CurrentComboAmplifyLevel;
	
};
