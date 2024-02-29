// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLLStringDataTable.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FStringDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Kor = 1;
};

UCLASS()
class PROJECT_LLL_API ULLLStringDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
