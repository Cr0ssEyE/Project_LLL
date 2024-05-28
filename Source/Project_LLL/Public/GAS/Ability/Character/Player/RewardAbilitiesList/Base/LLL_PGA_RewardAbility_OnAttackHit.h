// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_RewardAbility_OnAttackHit.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_RewardAbility_OnAttackHit : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	virtual void ApplyEffectWhenHit();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "트리거 발생시 요구 태그")
	FGameplayTag TriggerRequiredTag;
	
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "적중시 발생하는 GE")
	TSubclassOf<UGameplayEffect> OnAttackHitEffect;

};
