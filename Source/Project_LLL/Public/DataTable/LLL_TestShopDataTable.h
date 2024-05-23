// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_TestShopDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTestShopDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FTestShopDataTable()
	{
		AppearStage = 0;
		RewardGroupID = 0;
		RewardID = 0;
		ProductCount = 0;
		ProductPrice = 0;
		AppearRate = 0;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 AppearStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 RewardGroupID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 RewardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 ProductCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 ProductPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 AppearRate;
	
};

UCLASS()
class PROJECT_LLL_API ULLL_TestShopDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
