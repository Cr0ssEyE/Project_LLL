// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_TestAbilityDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FTestAbilityDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AbilityType = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AbilityParts = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AbilityRank = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString AbilityCategory = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AbilityValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float ChangeValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString RequireCategory = TEXT("");
};


UCLASS()
class PROJECT_LLL_API ULLL_TestAbilityDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
