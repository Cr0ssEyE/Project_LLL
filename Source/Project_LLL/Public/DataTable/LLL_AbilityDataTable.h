// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_AbilityDataTable.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None UMETA(Hidden),
	Crow,
	Deer,
	Wolf
};

UENUM(BlueprintType)
enum class EAbilityPart : uint8
{
	Common,
	Attack,
	Chase,
	Dash,
	ComboSkill
};

UENUM(BlueprintType)
enum class EAbilityRank : uint8
{
	Normal,
	Rare,
	Epic,
	Legend
};

UENUM(BlueprintType)
enum class EAbilityCategory : uint8
{
	Null,
	Feather,
	Critical,
	Marking,
	Bleeding
};

USTRUCT(BlueprintType)
struct FAbilityDataTable : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 ID = 10000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 유형")
	EAbilityType AbilityType = EAbilityType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 부위")
	EAbilityPart AbilityPart = EAbilityPart::Common;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 등급")
	EAbilityRank AbilityRank = EAbilityRank::Normal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 분류")
	EAbilityCategory AbilityCategory = EAbilityCategory::Null;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 이름")
	FName AbilityName = FName("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 정보")
	FString AbilityInformation = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "능력 수치")
	float AbilityValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "수치 변화량")
	float ChangeValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", DisplayName = "획득 조건")
	EAbilityCategory RequireCategory = EAbilityCategory::Null;
};

UCLASS()
class PROJECT_LLL_API ULLL_AbilityDataTable : public UDataTable
{
	GENERATED_BODY()
};
