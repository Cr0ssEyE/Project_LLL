﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackBase.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitGameplayTagAdded;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_AttackBase : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_AttackBase();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void CheckInputPressed(FGameplayEventData EventData);
	
	void SetNextAttackAction();

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitTagTask;
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	uint32 CurrentComboAction;

	uint32 MaxAttackAction;
	
	uint8 bIsCanPlayNextAction : 1;
};
