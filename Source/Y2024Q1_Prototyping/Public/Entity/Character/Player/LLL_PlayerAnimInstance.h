// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"
#include "LLL_PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAttackHitCheckDelegate, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FAttackComboCheckDelegate, bool)
DECLARE_MULTICAST_DELEGATE(FDeadMotionEndedDelegate)
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerAnimInstance : public ULLL_BaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_PlayerAnimInstance();

	FAttackHitCheckDelegate AttackHitCheckDelegate;
	FAttackComboCheckDelegate AttackComboCheckDelegate;
	FDeadMotionEndedDelegate DeadMotionEndedDelegate;
	
private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackHitCheckStart() { AttackHitCheckDelegate.Broadcast(true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackHitCheckEnd() { AttackHitCheckDelegate.Broadcast(false); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackComboCheckStart() { AttackComboCheckDelegate.Broadcast(true); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackComboCheckEnd() { AttackComboCheckDelegate.Broadcast(false); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_DeadMotionEnded() { DeadMotionEndedDelegate.Broadcast(); }
};
