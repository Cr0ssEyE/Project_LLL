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
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 충돌 사이즈(높이, 반지름)")
	FVector2D MonsterCollisionSize;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "기본 이동속도")
	float MonsterBaseMoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "기본 가속도")
	float MonsterBaseAccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "방향전환 속도")
	float MonsterBaseTurnSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "지면 마찰력")
	float MonsterBaseGroundFriction;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "블랙보드")
	TObjectPtr<UBlackboardData> MonsterBaseBlackBoard;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "행동트리")
	TObjectPtr<UBehaviorTree> MonsterBaseBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "감지 거리")
	float DetectDistance;

	UPROPERTY(EditDefaultsOnly, Category = "AI", DisplayName = "시야각")
	float FieldOfView;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", DisplayName = "공격 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimationMontage;
};
