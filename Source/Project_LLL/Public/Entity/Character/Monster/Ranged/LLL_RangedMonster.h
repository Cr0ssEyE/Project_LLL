// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_RangedMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "LLL_RangedMonster.generated.h"

class ULLL_ObjectPoolingComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_RangedMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_RangedMonster();
	
	void ThrowToPlayer();
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_RangedMonsterDataAsset> RangedMonsterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ObjectPoolingComponent> ObjectPooling;
};
