// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_BaseCharacterDataAsset.h"
#include "LLL_MonsterBaseDataAsset.generated.h"

class UBehaviorTree;
class UBlackboardData;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterBaseDataAsset : public ULLL_BaseCharacterDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "블랙보드")
	TObjectPtr<UBlackboardData> BlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "행동트리")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 게이지 위치")
	FVector StatusGaugeLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", DisplayName = "스테이터스 게이지 크기")
	FVector2D StatusGaugeSize;
};
