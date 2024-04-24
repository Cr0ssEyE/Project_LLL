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
	None UMETA(Hidden, DisplayName = "None"),
	Crow UMETA(DisplayName = "Crow"),
	Deer UMETA(DisplayName = "Deer"),
	Wolf UMETA(DisplayName = "Wolf")
};

UENUM(BlueprintType)
enum class EAbilityPart : uint8
{
	Common UMETA(DisplayName = "Common"),
	Attack UMETA(DisplayName = "Attack"),
	Chase UMETA(DisplayName = "Chase"),
	Dash UMETA(DisplayName = "Dash"),
	ComboSkill UMETA(DisplayName = "ComboSkill")
};

UENUM(BlueprintType)
enum class EAbilityRank : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legend UMETA(DisplayName = "Legend")
};

UENUM(BlueprintType)
enum class EAbilityCategory : uint8
{
	Null UMETA(DisplayName = "Null"),
	Feather UMETA(DisplayName = "Feather"),
	Critical UMETA(DisplayName = "Critical"),
	Marking UMETA(DisplayName = "Marking"),
	Bleeding UMETA(DisplayName = "Bleeding")
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
