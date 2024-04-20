// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_MonsterSpawnDataTable.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_MonsterSpawner.generated.h"

class ALLL_MonsterBase;
class UBoxComponent;
class ULLL_MonsterSpawnDataTable;
class ULLL_MonsterSpawnPointComponent;

DECLARE_MULTICAST_DELEGATE(FMonsterSpawnerDestroyDelegate)
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_MonsterSpawner : public ALLL_SystemBase
{
	GENERATED_BODY()

public:
	ALLL_MonsterSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void BeginDestroy() override;
	
public:
	void SpawnMonster();

	FMonsterSpawnerDestroyDelegate MonsterSpawnerDestroyDelegate;
	
private:
	UFUNCTION()
	void MonsterDeadHandle(ALLL_BaseCharacter* BaseCharacter);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<const UDataTable> MonsterSpawnDataTable;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FMonsterSpawnDataTable> MonsterSpawnDataArray;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<ULLL_MonsterSpawnPointComponent*> SpawnPoints;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DetectBox;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<ALLL_MonsterBase*> Monsters;
	
	UPROPERTY(VisibleDefaultsOnly)
	int32 CurrentWave;

	UPROPERTY(EditAnywhere)
	int32 MaxWave;

	UPROPERTY(VisibleDefaultsOnly)
	int32 CurrentGroup;

	UPROPERTY(VisibleDefaultsOnly)
	int32 LastGroup;
};
