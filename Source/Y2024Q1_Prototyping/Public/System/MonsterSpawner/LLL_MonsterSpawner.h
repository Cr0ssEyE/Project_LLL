// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_MonsterSpawner.generated.h"

struct FMonsterSpawnDataTable;
class ULLL_MonsterSpawnDataTable;
class ULLL_MonsterSpawnPointComponent;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_MonsterSpawner : public AActor
{
	GENERATED_BODY()

public:
	ALLL_MonsterSpawner();
	
	void SpawnMonster();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
	TArray<ULLL_MonsterSpawnPointComponent*> SpawnPoints;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const UDataTable> MonsterSpawnDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	int32 Wave;

	UPROPERTY(VisibleDefaultsOnly)
	int32 Group;
};
