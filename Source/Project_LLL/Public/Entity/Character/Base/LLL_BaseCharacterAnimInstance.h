// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "LLL_BaseCharacterAnimInstance.generated.h"

class ALLL_BaseCharacter;
class UCharacterMovementComponent;

DECLARE_MULTICAST_DELEGATE(FDeadMotionEndedDelegate)
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_BaseCharacterAnimInstance();
	
	//FORCEINLINE virtual void SetDataAsset(const ULLL_BaseCharacterDataAsset* InCharacterDataAsset) { CharacterDataAsset = InCharacterDataAsset; }

	void PlayDeadAnimation();

	FDeadMotionEndedDelegate DeadMotionEndedDelegate;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void AnimNotify_DeadMotionEnded() { DeadMotionEndedDelegate.Broadcast(); }
	
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	uint8 bIsIdle : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	uint8 bIsFalling : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	uint8 bIsJumping : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float MovingThreshold;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float JumpingThreshold;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ALLL_BaseCharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> Movement;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BaseCharacterDataAsset> CharacterDataAsset;
};
