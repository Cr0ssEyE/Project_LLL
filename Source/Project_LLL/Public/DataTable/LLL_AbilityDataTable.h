// Fill out your copyright notice in the Description page of Project Settings.

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
	AbilityType(EAbilityType::None),
	AbilityPart(EAbilityPart::Common),
	AbilityRank(EAbilityRank::Normal),
	AbilityCategory(EAbilityCategory::Null),
	AbilityValue(0.f),
	ChangeValue(0.f),
	RequireCategory(EAbilityCategory::Null)
	{

	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 유형")
	EAbilityType AbilityType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 부위")
	EAbilityPart AbilityPart;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 등급")
	EAbilityRank AbilityRank;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 분류")
	EAbilityCategory AbilityCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 이름")
	FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 정보")
	FString AbilityInformation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 수치")
	float AbilityValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "수치 변화량")
	float ChangeValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "획득 조건")
	EAbilityCategory RequireCategory;
};

UCLASS()
class PROJECT_LLL_API ULLL_AbilityDataTable : public UDataTable
{
	GENERATED_BODY()
};
