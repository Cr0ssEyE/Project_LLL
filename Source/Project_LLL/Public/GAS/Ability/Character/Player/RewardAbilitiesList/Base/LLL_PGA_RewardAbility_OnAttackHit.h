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
	UFUNCTION()
	void OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

	UFUNCTION()
	void OnTraceEndCallBack(FGameplayEventData EventData);
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> OnAttackHitEffects;
};
