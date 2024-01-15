// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "LLL_MeleeMonster.generated.h"

class ULLL_MeleeMonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MeleeMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_MeleeMonster();
	
	virtual bool AttackAnimationIsPlaying() override;
	
	void DamageToPlayer();
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MeleeMonsterDataAsset> MeleeMonsterDataAsset;
};
