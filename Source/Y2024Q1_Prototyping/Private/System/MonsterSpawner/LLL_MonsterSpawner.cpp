// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_MonsterSpawner.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_MonsterSpawnerArraySize.h"
#include "DataTable/LLL_MonsterSpawnDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "System/MonsterSpawner/LLL_MonsterSpawnPointComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MonsterSpawner::ALLL_MonsterSpawner()
{
	MonsterSpawnDataTable = FLLLConstructorHelper::FindAndGetObject<ULLL_MonsterSpawnDataTable>(PATH_MONSTER_SPAWN_DATA, EAssertionLevel::Check);
	MonsterSpawnDataTable->GetAllRows<FMonsterSpawnDataTable>(TEXT("Failed To Load Monster Spawn Data Tables"), DataTables);
}

void ALLL_MonsterSpawner::SpawnMonster()
{
	Wave++;
	
	Group = FMath::RandRange(1, MS_GROUPS_SIZE);
	int32 SpawnPointNum = 0;
	
	for (ULLL_MonsterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPointNum++;

			for (FMonsterSpawnDataTable DataTable : DataTables)
			{
				if (DataTable.Group == Group && DataTable.SpawnPoint == SpawnPointNum)
				{
					GetWorld()->SpawnActor<ALLL_MonsterBase>(DataTable.MonsterClass);
				}
			}
		}
	}
}

void ALLL_MonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	for (USceneComponent* ChildComponent : GetRootComponent()->GetAttachChildren())
	{
		ULLL_MonsterSpawnPointComponent* SpawnPoint = Cast<ULLL_MonsterSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}

	SpawnMonster();
}
