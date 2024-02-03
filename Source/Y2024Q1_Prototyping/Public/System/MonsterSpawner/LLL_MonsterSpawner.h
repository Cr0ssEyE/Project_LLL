// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "LLL_MonsterSpawner.generated.h"

class ALLL_MonsterBase;
class UBoxComponent;
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

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	void SpawnMonster();
	
private:
	UFUNCTION()
	void MonsterDeadHandle(ALLL_BaseCharacter* BaseCharacter);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const UDataTable> MonsterSpawnDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FMonsterSpawnDataTable> DataTables;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<ULLL_MonsterSpawnPointComponent*> SpawnPoints;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DetectBox;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<ALLL_MonsterBase*> Monsters;
	
	UPROPERTY(VisibleAnywhere)
	int32 Wave;

	UPROPERTY(VisibleDefaultsOnly)
	int32 MaxWave;

	UPROPERTY(VisibleDefaultsOnly)
	int32 Group;

	UPROPERTY(VisibleDefaultsOnly)
	int32 LastGroup;
};
