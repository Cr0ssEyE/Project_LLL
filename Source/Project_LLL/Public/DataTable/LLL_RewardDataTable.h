// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_RewardDataTable.generated.h"


UENUM()
enum class ERewardCategory : uint8
{
	Gold = 1,
	Ability,
	Enhance,
	MaxHP
};

USTRUCT(BlueprintType)
struct FRewardDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 GroupID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName DropType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName UseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FName GetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 Value;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	bool bIsHardReward = false;
};


UCLASS()
class PROJECT_LLL_API ULLL_RewardDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
