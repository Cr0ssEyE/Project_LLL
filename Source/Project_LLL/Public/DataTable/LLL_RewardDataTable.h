// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_RewardDataTable.generated.h"

/**
 * 
 */
UENUM()
enum class ERewardType : uint8
{
	Ability,
	Gold,
	MaxHealth,
	Default,
};

USTRUCT(BlueprintType)
struct FRewardDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ERewardType RewardType = ERewardType::Default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float RewardValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsHardReward = false;
};


UCLASS()
class PROJECT_LLL_API ULLL_RewardDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
