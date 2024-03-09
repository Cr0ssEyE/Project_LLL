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
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "피격 애님 몽타주")
	TObjectPtr<UAnimMontage> DamagedAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "블랙보드")
	TObjectPtr<UBlackboardData> BlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "행동트리")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "감지 거리")
	float DetectDistance;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "시야각")
	float FieldOfView;
};
