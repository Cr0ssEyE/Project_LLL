// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LLL_MonsterBaseAnimInstance.generated.h"

class ULLL_MonsterBaseDataAsset;

DECLARE_MULTICAST_DELEGATE(FAttackAnimationEndDelegate);
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_MonsterBaseAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;

public:
	void PlayAttackAnimation();

	FAttackAnimationEndDelegate AttackAnimationEndDelegate;

protected:
	void AnimNotify_Hit();
	
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	uint8 bAttackAnimationIsPlaying;
};
