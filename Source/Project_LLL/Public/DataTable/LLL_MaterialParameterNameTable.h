// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_MaterialParameterNameTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMaterialParameterNameTable : public FTableRowBase
{
	GENERATED_BODY()

	FMaterialParameterNameTable() {  }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName Name;
};

UCLASS()
class PROJECT_LLL_API ULLL_MaterialParameterNameTable : public UDataTable
{
	GENERATED_BODY()
};
