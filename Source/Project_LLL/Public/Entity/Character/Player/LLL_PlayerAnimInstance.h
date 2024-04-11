// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"
#include "LLL_PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAttackHitCheckDelegate, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FAttackComboCheckDelegate, bool)
DECLARE_MULTICAST_DELEGATE(FDeadMotionEndedDelegate)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerAnimInstance : public ULLL_BaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	FAttackHitCheckDelegate AttackHitCheckDelegate;
	FAttackComboCheckDelegate AttackComboCheckDelegate;
	FDeadMotionEndedDelegate DeadMotionEndedDelegate;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void AnimNotify_LeftStep() override;
	virtual void AnimNotify_RightStep() override;

public:
	void PlayDeadAnimation();

protected:
	void SetStepEventParameter(FName FootSocketName) const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_DeadMotionEnded() { DeadMotionEndedDelegate.Broadcast(); }

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	uint8 bIsDashing : 1;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_PlayerBaseDataAsset> PlayerDataAsset;
	
private:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackHitCheckStart() { AttackHitCheckDelegate.Broadcast(true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackHitCheckEnd() { AttackHitCheckDelegate.Broadcast(false); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackComboCheckStart() { AttackComboCheckDelegate.Broadcast(true); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_AttackComboCheckEnd() { AttackComboCheckDelegate.Broadcast(false); }
};
