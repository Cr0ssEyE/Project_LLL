// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_CharacterBaseDataAsset.h"
#include "LLL_MonsterDataAsset.generated.h"

class UBehaviorTree;
class UBlackboardData;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterDataAsset : public ULLL_CharacterBaseDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Base")
	TObjectPtr<UBlackboardData> MonsterBaseBlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit")
	TObjectPtr<UBehaviorTree> MonsterBaseBehaviorTree;
};
