// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_MonsterBase.generated.h"

class ULLL_MonsterDataAsset;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MonsterBase : public ALLL_BaseCharacter
{
	GENERATED_BODY()

public:
	ALLL_MonsterBase();

protected:
	UPROPERTY()
	TObjectPtr<const ULLL_MonsterDataAsset> MonsterDataAsset;
};
