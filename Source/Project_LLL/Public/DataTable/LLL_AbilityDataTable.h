﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "LLL_AbilityDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FAbilityDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FAbilityDataTable() :
	ID(10000),
	AnimalType(EAnimalType::None),
	AbilityRank(EAbilityRank::Normal),
	AbilityCategory(EAbilityCategory::Null),
	bIsImplement(false),
	AbilityValue1(0.f),
	Value1Type(EAbilityValueType::Fixed),
	AbilityValue2(0.f),
	Value2Type(EAbilityValueType::Fixed),
	KnockBackPower(0.f),
	AbilityCooldown(0.f),
	RequireSynergy(0),
	RequireCategory(EAbilityCategory::Null),
	GetAbilityRate(0.f)
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "동물 유형")
	EAnimalType AnimalType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 등급")
	EAbilityRank AbilityRank;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 분류")
	EAbilityCategory AbilityCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 이름")
	FString AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 정보")
	FString AbilityInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "구현 여부")
	uint8 bIsImplement : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 수치 1")
	float AbilityValue1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "수치 1 유형")
	EAbilityValueType Value1Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 수치 2")
	float AbilityValue2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "수치 2 유형")
	EAbilityValueType Value2Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "넉백 수치")
	float KnockBackPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 쿨타임")
	float AbilityCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "시너지 요구량")
	int32 RequireSynergy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "획득 조건")
	EAbilityCategory RequireCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "획득 가중치")
	int32 GetAbilityRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "태그 ID")
	FString TagID;
};

UCLASS()
class PROJECT_LLL_API ULLL_AbilityDataTable : public UDataTable
{
	GENERATED_BODY()
};
