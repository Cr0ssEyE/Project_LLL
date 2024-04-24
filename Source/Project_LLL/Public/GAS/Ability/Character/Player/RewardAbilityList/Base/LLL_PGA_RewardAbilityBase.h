// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_RewardAbilityBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
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

UCLASS()
class PROJECT_LLL_API ULLL_PGA_RewardAbilityBase : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	int32 ID;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 유형")
	EAbilityType AbilityType;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 부위")
	EAbilityPart AbilityPart;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 등급")
	EAbilityRank AbilityRank;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 분류")
	EAbilityCategory AbilityCategory;
	
	UPROPERTY(EditAnywhere, DisplayName = "능력 이름")
	FName AbilityName;

	UPROPERTY(EditAnywhere, DisplayName = "능력 설명")
	FText AbilityInformation;

	UPROPERTY(EditAnywhere, DisplayName = "획득 조건")
	EAbilityCategory RequireCategory;
};
