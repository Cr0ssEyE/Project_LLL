// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LLL_MonsterSpawnDataTable.generated.h"

/**
 * 
 */

class ALLL_MonsterBase;

USTRUCT(BlueprintType)
struct FMonsterSpawnDataTable : public FTableRowBase
{
	GENERATED_BODY()
	
	// 해당 FName과 일치하는 열의 값 읽어옴
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Group = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 SpawnPoint = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ALLL_MonsterBase> MonsterClass;
};

UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_MonsterSpawnDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
