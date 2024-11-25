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
	
	FMonsterSpawnDataTable() :
	Group(1),
	SpawnPoint(1),
	bIsElite(false)
	{
		
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Group;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ALLL_MonsterBase> MonsterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	uint8 bIsElite : 1;
};

UCLASS()
class PROJECT_LLL_API ULLL_MonsterSpawnDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
