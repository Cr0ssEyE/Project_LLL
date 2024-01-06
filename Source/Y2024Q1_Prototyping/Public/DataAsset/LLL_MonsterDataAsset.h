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
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "몬스터 캐릭터 충돌 사이즈(높이, 반지름)")
	FVector2D MonsterCollisionSize;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "몬스터 기본 이동속도")
	float MonsterBaseMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "몬스터 기본 가속도")
	float MonsterBaseAccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "몬스터 방향전환 속도")
	float MonsterBaseTurnSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "몬스터 지면 마찰력")
	float MonsterBaseGroundFriction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Base", DisplayName = "몬스터 블랙보드")
	TObjectPtr<UBlackboardData> MonsterBaseBlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "Rabbit", DisplayName = "몬스터 행동트리")
	TObjectPtr<UBehaviorTree> MonsterBaseBehaviorTree;
};
