// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_AttackBase.generated.h"

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
	void SetNextAttackAction();
	void StartAttackInputWait();
	void EndAttackInputWait();
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	FTimerHandle WaitInputTimerHandle;

	float AttackActionIntervalTime;

	float AttackActionInputDelayTime;
	
	uint32 CurrentComboAction;

	uint32 MaxAttackAction;
	
	uint8 bIsInputPressed : 1;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "공격 이벤트 태그", meta=(Categories = "GameplayCue"))
	FGameplayTag AttackCueTag;

	FName PlayerWalkMaterialParameterName;
};
