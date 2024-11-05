// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"
#include "LLL_PlayerAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FDeadMotionEndedDelegate)

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerAnimInstance : public ULLL_BaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetDash(bool IsDashing) { bIsDashing = IsDashing; }
	FORCEINLINE bool IsDashing() const { return bIsDashing; }

	FDeadMotionEndedDelegate DeadMotionEndedDelegate;
	
protected:
	virtual void NativeInitializeAnimation() override;

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_DeadMotionEnded() { DeadMotionEndedDelegate.Broadcast(); }

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	uint8 bIsDashing : 1;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_PlayerBaseDataAsset> PlayerDataAsset;
};
