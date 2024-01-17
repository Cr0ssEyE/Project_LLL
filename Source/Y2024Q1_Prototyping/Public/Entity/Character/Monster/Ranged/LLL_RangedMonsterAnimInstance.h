// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "LLL_RangedMonsterAnimInstance.generated.h"

class ULLL_RangedMonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_RangedMonsterAnimInstance : public ULLL_MonsterBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void PlayAttackAnimation() override;

protected:
	UFUNCTION()
	void AnimNotify_Release();
};
