// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LLL_CharacterBaseDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_CharacterBaseDataAsset : public UDataAsset
{
	GENERATED_BODY()
	// 캐릭터 기본 모델링 관련
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 메시")
	TObjectPtr<USkeletalMesh> CharacterBaseMesh;

	// 각 몬스터 별 캐스팅이 필요한 경우 불러올 때 각 클래스 내에 변수 만들어서 받아놓기
	UPROPERTY(EditDefaultsOnly, Category = "Character", DisplayName = "캐릭터 애님 블루프린트")
	TSubclassOf<UAnimInstance> CharacterAnimBlueprint;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "캐릭터 기본 체력")
	uint32 CharacterBaseHealthAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "캐릭터 기본 보호막")
	uint32 CharacterBaseShieldAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "캐릭터 기본 공격력")
	uint32 CharacterBaseOffensePower; 

	UPROPERTY(EditDefaultsOnly, Category = "Status", DisplayName = "캐릭터 기본 이동속도")
	uint32 CharacterBaseMoveSpeed;
	
};
