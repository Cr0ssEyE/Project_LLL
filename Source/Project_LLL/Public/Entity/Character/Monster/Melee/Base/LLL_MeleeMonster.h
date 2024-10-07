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
class PROJECT_LLL_API ALLL_MeleeMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_MeleeMonster();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_MeleeMonsterDataAsset> MeleeMonsterDataAsset;
};
