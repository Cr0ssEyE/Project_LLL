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
