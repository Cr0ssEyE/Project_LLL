// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_OnSkillActivate.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_OnSkillActivate : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_OnSkillActivate();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	virtual void ApplyEffectWhenActivateSuccess();

	UFUNCTION()
	virtual void OnSkillDeactivatedCallBack(FGameplayEventData EventData);
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTag TriggerRequiredTag;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적용 GE")
	TSubclassOf<UGameplayEffect> OnActivateSuccessEffect;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적용 대상")
	EEffectApplyTarget EffectApplyTarget;
	
};
