// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DataAsset/LLL_BaseCharacterDataAsset.h"
#include "LLL_BaseCharacterAnimInstance.generated.h"

class ALLL_BaseCharacter;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMontageEndedEnhancedDelegate, ALLL_BaseCharacter*, Character, UAnimMontage*, Montage, bool, bIsInterrupt); 

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_BaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_BaseCharacterAnimInstance();

	FMontageEndedEnhancedDelegate MontageEndedEnhancedDelegate;
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION(BlueprintCallable)
	virtual void MontageEndedEvent(UAnimMontage* Montage, const bool bIsInterrupt);
	
	// 사운드 관련
protected:
	UFUNCTION(BlueprintCallable)
	virtual void AnimNotify_LeftStep();

	UFUNCTION(BlueprintCallable)
	virtual void AnimNotify_RightStep();

	void ExecuteStepCue() const;

protected:
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

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ALLL_BaseCharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<UCharacterMovementComponent> Movement;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BaseCharacterDataAsset> CharacterDataAsset;
};
