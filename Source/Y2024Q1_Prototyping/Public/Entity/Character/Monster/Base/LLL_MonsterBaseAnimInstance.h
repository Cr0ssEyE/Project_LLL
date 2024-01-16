// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Entity/Character/Base/LLL_BaseCharacterAnimInstance.h"
#include "LLL_MonsterBaseAnimInstance.generated.h"

class ULLL_MonsterBaseDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterBaseAnimInstance : public ULLL_BaseCharacterAnimInstance
{
	GENERATED_BODY()

public:
	virtual void PlayAttackAnimation() {}

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MonsterBaseDataAsset> MonsterBaseDataAsset;
};
