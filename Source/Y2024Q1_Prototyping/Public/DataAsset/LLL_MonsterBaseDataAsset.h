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
class Y2024Q1_PROTOTYPING_API ULLL_MonsterBaseDataAsset : public ULLL_CharacterBaseDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "블랙보드")
	TObjectPtr<UBlackboardData> BlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "행동트리")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "감지 거리")
	float DetectDistance;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "시야각")
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", DisplayName = "공격 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimationMontage;
};
