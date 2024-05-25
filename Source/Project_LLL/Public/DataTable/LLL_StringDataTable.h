// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_StringDataTable.generated.h"

USTRUCT(BlueprintType)
struct FStringDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	FStringDataTable() :
		ID(""),
		Korean("")
	{
		
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FString Korean;
	
};

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_StringDataTable : public UDataTable
{
	GENERATED_BODY()
};
