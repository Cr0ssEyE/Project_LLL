// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enumeration/LLL_FModParameterHelper.h"
#include "LLL_FModParameterDataTable.generated.h"

/**
 * 
 */

enum class EFModParameter : uint8;

USTRUCT(BlueprintType)
struct FFModParameterDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameter")
	EFModParameter Parameter = EFModParameter::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FName Name;
};

UCLASS()
class PROJECT_LLL_API ULLL_FModParameterDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
