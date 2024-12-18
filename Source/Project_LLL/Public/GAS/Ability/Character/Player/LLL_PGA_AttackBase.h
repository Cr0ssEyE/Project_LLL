﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackBase.generated.h"

class UFMODEvent;
class UNiagaraSystem;
class ALLL_PlayerBase;
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

	UFUNCTION()
	void CheckFullCharge(FGameplayEventData EventData);

	void BaseAttack();
	void SetNextAttackAction();
	void ChargeAttack();
	void ChargeRotate(ALLL_PlayerBase* Player);
	float GetFullChargeNotifyTriggerTime(bool Range) const;
	void ChargeEnd();

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitTagTask;
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	UPROPERTY(EditAnywhere, DisplayName = "충전 공격 애님 몽타주")
	TObjectPtr<UAnimMontage> ChargeAttackAnimMontage;

	UPROPERTY(EditAnywhere, DisplayName = "제거할 차징 이펙트 1")
	TObjectPtr<UNiagaraSystem> ChargeParticle1;
	
	UPROPERTY(EditAnywhere, DisplayName = "제거할 차징 이펙트 1")
	TObjectPtr<UNiagaraSystem> ChargeParticle2;

	UPROPERTY(EditAnywhere, DisplayName = "차징 1 FMode")
	TObjectPtr<UFMODEvent> ChargeAttack1FModEvent;

	UPROPERTY(EditAnywhere, DisplayName = "차징 2 FMod")
	TObjectPtr<UFMODEvent> ChargeAttack2FModEvent;

	uint32 CurrentComboAction;
	uint32 MaxAttackAction;
	uint8 bIsCanPlayNextAction : 1;
	uint8 bStopCharge : 1;
	uint8 bFullCharged : 1;
};
