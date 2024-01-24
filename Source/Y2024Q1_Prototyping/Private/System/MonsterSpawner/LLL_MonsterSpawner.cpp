// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_MonsterSpawner.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataTable/LLL_MonsterSpawnDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "System/MonsterSpawner/LLL_MonsterSpawnPointComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MonsterSpawner::ALLL_MonsterSpawner()
{
	MonsterSpawnDataTable = FLLLConstructorHelper::FindAndGetObject<UDataTable>(PATH_MONSTER_SPAWN_DATA, EAssertionLevel::Check);
	
	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DetectBox->SetCollisionObjectType(ECC_PLAYER_ONLY);
	DetectBox->OnComponentBeginOverlap.AddDynamic(this, &ALLL_MonsterSpawner::PlayerDetectHandle);
	
	SetRootComponent(DetectBox);

	MaxWave = 3;
}

void ALLL_MonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMonsterSpawnDataTable*> LoadDataTables;
	MonsterSpawnDataTable->GetAllRows<FMonsterSpawnDataTable>(TEXT("Failed To Load Monster Spawn Data Tables"), LoadDataTables);

	int rowNum = 0;
	for (const FMonsterSpawnDataTable* LoadDataTable : LoadDataTables)
	{
		FMonsterSpawnDataTable TempDataTable;
		TempDataTable.Group = LoadDataTable->Group;
		TempDataTable.SpawnPoint = LoadDataTable->SpawnPoint;
		TempDataTable.MonsterClass = LoadDataTable->MonsterClass;
		DataTables.Emplace(TempDataTable);

		rowNum++;
		if (rowNum == LoadDataTables.Num())
		{
			LastGroup = LoadDataTable->Group;
		}
	}
	
	for (USceneComponent* ChildComponent : GetRootComponent()->GetAttachChildren())
	{
		ULLL_MonsterSpawnPointComponent* SpawnPoint = Cast<ULLL_MonsterSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}

	DetectBox->OnComponentBeginOverlap.AddDynamic(this, &ALLL_MonsterSpawner::PlayerDetectHandle);
}

void ALLL_MonsterSpawner::SpawnMonster()
{
	Wave++;
	
	Group = FMath::RandRange(1, LastGroup);
	int32 SpawnPointNum = 0;

	for (const ULLL_MonsterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPointNum++;

			for (FMonsterSpawnDataTable DataTable : DataTables)
			{
				if (DataTable.Group == Group && DataTable.SpawnPoint == SpawnPointNum)
				{
					ALLL_MonsterBase* MonsterBase = GetWorld()->SpawnActor<ALLL_MonsterBase>(DataTable.MonsterClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
					if (IsValid(MonsterBase))
					{
						MonsterBase->CharacterDeadDelegate.AddUObject(this, &ALLL_MonsterSpawner::MonsterDeadHandle);
						Monsters.Emplace(MonsterBase);
					}
				}
			}
		}
	}
}

void ALLL_MonsterSpawner::PlayerDetectHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Wave == 0)
	{
		const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor);
		if (IsValid(Player))
		{
			SpawnMonster();
		}
	}
}

void ALLL_MonsterSpawner::MonsterDeadHandle(ALLL_BaseCharacter* BaseCharacter)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(BaseCharacter);
	if (IsValid(Monster))
	{
		Monsters.Remove(Monster);
	}

	if (Monsters.Num() == 0)
	{
		if (Wave < MaxWave)
		{
			SpawnMonster();
		}
		else
		{
			Destroy();
		}
	}
}
