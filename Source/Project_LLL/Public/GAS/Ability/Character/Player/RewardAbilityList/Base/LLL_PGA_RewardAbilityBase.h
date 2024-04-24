// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_RewardAbilityBase.generated.h"

/**
 * 
 */

enum class EAbilityCategory : uint8;
enum class EAbilityRank : uint8;
enum class EAbilityPart : uint8;
enum class EAbilityType : uint8;

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

	UPROPERTY(EditAnywhere, DisplayName = "능력 정보")
	FString AbilityInformation;

	UPROPERTY(EditAnywhere, DisplayName = "능력 수치")
	float AbilityValue;

	UPROPERTY(EditAnywhere, DisplayName = "수치 변화량")
	float ChangeValue;

	UPROPERTY(EditAnywhere, DisplayName = "획득 조건")
	EAbilityCategory RequireCategory;
};
