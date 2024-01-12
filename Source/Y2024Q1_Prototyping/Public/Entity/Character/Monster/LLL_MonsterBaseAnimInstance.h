// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LLL_MonsterBaseAnimInstance.generated.h"

class ULLL_MonsterBaseDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULLL_MonsterBaseAnimInstance();

	void PlayAttackAnimation();

protected:
	void AnimNotify_Hit();
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;
};
