// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_RewardDataTable.generated.h"

USTRUCT(BlueprintType)
struct FRewardDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FRewardDataTable() :
	GroupID(0),
	ID(INT8_MAX),
	Value(0),
	bIsHardReward(false)
	{
		
	}
	
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
	bool bIsHardReward;
};


UCLASS()
class PROJECT_LLL_API ULLL_RewardDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
