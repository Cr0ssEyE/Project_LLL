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
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "메시")
	TObjectPtr<USkeletalMesh> CharacterBaseMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 충돌 사이즈(높이, 반지름)")
	FVector2D CollisionSize;

	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "애님 블루프린트")
	TSoftClassPtr<UAnimInstance> CharacterAnimInstance;

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
	uint32 CharacterBaseHealthAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 보호막")
	uint32 CharacterBaseShieldAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 공격력")
	uint32 CharacterBaseOffensePower; 

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "기본 이동속도")
	uint32 CharacterBaseMoveSpeed;
	
};
