// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_AttackInApnea.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_AttackInApnea : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void AttackMiddle(int32 AttackCount);
	void AttackEnd();
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackInApneaStartMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackInApneaMiddleMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackInApneaEndMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float StartMontageDashTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float MiddleMontageDashTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Timer")
	float EndMontageDashTimer;
};
