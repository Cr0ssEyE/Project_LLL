// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_MonsterSpawner.h"

#include "FMODAudioComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataTable/LLL_MonsterSpawnDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "System/MonsterSpawner/LLL_MonsterSpawnPointComponent.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_MonsterSpawner::ALLL_MonsterSpawner()
{
	MonsterSpawnerDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MonsterSpawnerDataAsset>(PATH_MONSTER_SPAWNER_DATA, EAssertionLevel::Check);
	MonsterSpawnDataTable = FLLL_ConstructorHelper::FindAndGetObject<UDataTable>(PATH_MONSTER_SPAWN_DATA, EAssertionLevel::Check);
	
	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DetectBox->SetCollisionProfileName(CP_INTERACTION);
	
	SetRootComponent(DetectBox);
}

void ALLL_MonsterSpawner::BeginPlay()
{
	Super::BeginPlay();

	TArray<FMonsterSpawnDataTable*> LoadSpawnDataArray;
	MonsterSpawnDataTable->GetAllRows<FMonsterSpawnDataTable>(TEXT("Failed To Load Monster Spawn Data Tables"), LoadSpawnDataArray);

	int rowNum = 0;
	for (const FMonsterSpawnDataTable* LoadSpawnData : LoadSpawnDataArray)
	{
		FMonsterSpawnDataTable TempSpawnData;
		TempSpawnData.Group = LoadSpawnData->Group;
		TempSpawnData.SpawnPoint = LoadSpawnData->SpawnPoint;
		TempSpawnData.MonsterClass = LoadSpawnData->MonsterClass;
		MonsterSpawnDataArray.Emplace(TempSpawnData);

		rowNum++;
		if (rowNum == LoadSpawnDataArray.Num())
		{
			LastGroup = LoadSpawnData->Group;
		}
	}

	const USceneComponent* SpawnPointGroupComponent = GetRootComponent();
	if (GetAttachParentActor())
	{
		SpawnPointGroupComponent = GetAttachParentActor()->GetRootComponent();
	}
	
	for (USceneComponent* ChildComponent : SpawnPointGroupComponent->GetAttachChildren())
	{
		ULLL_MonsterSpawnPointComponent* SpawnPoint = Cast<ULLL_MonsterSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}
}

void ALLL_MonsterSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (CurrentWave == 0)
	{
		const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor);
		if (IsValid(Player))
		{
			SpawnMonster();
		}
	}
}

void ALLL_MonsterSpawner::BeginDestroy()
{
	Super::BeginDestroy();

	MonsterSpawnerDestroyDelegate.Broadcast();
}

void ALLL_MonsterSpawner::SpawnMonster()
{
	CurrentWave++;
	
	CurrentGroup++;
	if (CurrentGroup > LastGroup)
	{
		CurrentGroup -= LastGroup;
	}
	
	int32 SpawnPointNum = 0;

	for (const ULLL_MonsterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		if (IsValid(SpawnPoint))
		{
			SpawnPointNum++;

			for (const FMonsterSpawnDataTable MonsterSpawnData : MonsterSpawnDataArray)
			{
				if (MonsterSpawnData.Group == CurrentGroup && MonsterSpawnData.SpawnPoint == SpawnPointNum && IsValid(MonsterSpawnData.MonsterClass))
				{
					FTimerHandle MonsterSpawnTimerHandle;
					GetWorldTimerManager().SetTimer(MonsterSpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, MonsterSpawnData, SpawnPoint, SpawnPointNum]{
						ALLL_MonsterBase* MonsterBase = GetWorld()->SpawnActor<ALLL_MonsterBase>(MonsterSpawnData.MonsterClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
						if (IsValid(MonsterBase))
						{
							MonsterBase->CharacterDeadDelegate.AddDynamic(this, &ALLL_MonsterSpawner::MonsterDeadHandle);
							Monsters.Emplace(MonsterBase);
							UE_LOG(LogTemp, Log, TEXT("%f"), MonsterBase->CustomTimeDilation)
						
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
							if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
							{
								if (ProtoGameInstance->CheckMonsterSpawnDataDebug())
								{
									GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 스폰 (웨이브 : %d, 그룹 : %d, 스폰 포인트 : %d)"), *GetName(), CurrentWave, CurrentGroup, SpawnPointNum));
								}
							}
						}
#endif
					}), MonsterSpawnerDataAsset->SpawnTimer, false);

					FTimerHandle SpawnParticleTimerHandle;
					GetWorldTimerManager().SetTimer(SpawnParticleTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, SpawnPoint]{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MonsterSpawnerDataAsset->SpawnParticle, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
					}), MonsterSpawnerDataAsset->SpawnParticleTimer, false);
				}
			}
		}
	}

	FModAudioComponent->Release();
	FModAudioComponent->SetEvent(MonsterSpawnerDataAsset->SpawnSoundEvent);
	FModAudioComponent->Play();
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
		if (CurrentWave < MaxWave)
		{
			SpawnMonster();
		}
		else
		{
			Destroy();
		}
	}
}
