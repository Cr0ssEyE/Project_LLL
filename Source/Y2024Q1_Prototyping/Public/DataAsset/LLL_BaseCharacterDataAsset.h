// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_BaseCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_CharacterBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
	// 캐릭터 기본 모델링 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "메시", meta=(DisplayPriority=1))
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "애님 블루프린트", meta=(DisplayPriority=1))
	TSoftClassPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "공격 애님 몽타주")
	TObjectPtr<UAnimMontage> AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "사망 애니메이션")
	TObjectPtr<UAnimationAsset> DeadAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "충돌 사이즈(높이, 반지름)", meta=(DisplayPriority=1))
	FVector2D CollisionSize;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "기본 이동속도")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "기본 가속도")
	float AccelerateSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "방향전환 속도")
	float TurnSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement", DisplayName = "지면 마찰력")
	float GroundFriction;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 체력")
	uint32 Health;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 보호막")
	uint32 ShieldAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 공격력")
	uint32 OffensePower;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 공격 거리")
	float AttackDistance;
};
