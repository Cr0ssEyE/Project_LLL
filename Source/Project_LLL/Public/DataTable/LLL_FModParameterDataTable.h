// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "LLL_FModParameterDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FFModParameterDataTable : public FTableRowBase
{
	GENERATED_BODY()

	FFModParameterDataTable() : Parameter(EFModParameter::None) {  }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	EFModParameter Parameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName Name;
};

UCLASS()
class PROJECT_LLL_API ULLL_FModParameterDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
