// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_BossMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "LLL_BossMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_BossMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_BossMonster();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_BossMonsterDataAsset> BossMonsterDataAsset;
};
