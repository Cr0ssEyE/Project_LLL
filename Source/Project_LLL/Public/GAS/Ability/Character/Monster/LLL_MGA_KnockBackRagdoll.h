// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_KnockBackRagdoll.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_KnockBackRagdoll : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_MGA_KnockBackRagdoll();
	
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void RestoreRagdollState();
	
protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "래그돌 시작 애님 몽타주")
	TObjectPtr<UAnimMontage> RagdollBeginMontage;
	
	UPROPERTY(EditDefaultsOnly, DisplayName = "래그돌 유지 시간")
	float RagdollStateDuration;

	FTimerHandle RagdollTimerHandle;

	float CurrentBlendWeight;
};
