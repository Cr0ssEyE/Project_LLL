// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackBase.generated.h"

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
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	UFUNCTION()
	void WaitInputForNextAction();
	
	void SetNextAttackAction();
	void EndAttackInputWait();
	void ExecuteAttackCueWithDelay();

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayTagAdded> WaitTagTask;
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	FTimerHandle WaitInputTimerHandle;
	
	uint32 CurrentComboAction;

	uint32 MaxAttackAction;
	
	uint8 bIsCanPlayNextAction : 1;
	
	uint8 bIsInputPressed : 1;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 이벤트 큐 태그", meta=(Categories = "GameplayCue"))
	FGameplayTag AttackCueTag;

	UPROPERTY(EditAnywhere, DisplayName = "공격 모션 별 이벤트 큐 딜레이")
	TArray<float> AttackCueDelayArray;

	FName PlayerAttackCountParameterName;
};
