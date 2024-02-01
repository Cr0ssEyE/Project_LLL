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
class Y2024Q1_PROTOTYPING_API ALLL_RangedMonster : public ALLL_MonsterBase
{
	GENERATED_BODY()

public:
	ALLL_RangedMonster();

protected:
	virtual void BeginPlay() override;
	
public:
	void ThrowToPlayer();
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_RangedMonsterDataAsset> RangedMonsterDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ObjectPoolingComponent> ObjectPooling;
};
