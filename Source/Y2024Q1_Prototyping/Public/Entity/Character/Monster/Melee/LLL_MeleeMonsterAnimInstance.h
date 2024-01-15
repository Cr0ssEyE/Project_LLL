// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "LLL_MeleeMonsterAnimInstance.generated.h"

class ULLL_MeleeMonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MeleeMonsterAnimInstance : public ULLL_MonsterBaseAnimInstance
{
	GENERATED_BODY()
	
public:
	ULLL_MeleeMonsterAnimInstance();
	
	virtual void PlayAttackAnimation() override;

protected:
	UFUNCTION()
	void AnimNotify_Hit();

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MeleeMonsterDataAsset> MeleeMonsterDataAsset;
};
