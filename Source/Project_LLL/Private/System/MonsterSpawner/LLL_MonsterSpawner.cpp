// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_MonsterSpawner.h"

#include "FMODAudioComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataTable/LLL_MonsterSpawnDataTable.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "NiagaraFunctionLibrary.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
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

	int RowNum = 0;
	for (const FMonsterSpawnDataTable* LoadSpawnData : LoadSpawnDataArray)
	{
		FMonsterSpawnDataTable TempSpawnData;
		TempSpawnData.Group = LoadSpawnData->Group;
		TempSpawnData.SpawnPoint = LoadSpawnData->SpawnPoint;
		TempSpawnData.MonsterClass = LoadSpawnData->MonsterClass;
		MonsterSpawnDataArray.Emplace(TempSpawnData);

		RowNum++;
		if (RowNum == LoadSpawnDataArray.Num())
		{
			LastGroup = LoadSpawnData->Group;
		}
	}

	const USceneComponent* SpawnPointGroupComponent = GetRootComponent();
	if (const AActor* ParentActor = GetAttachParentActor())
	{
		SpawnPointGroupComponent = ParentActor->GetRootComponent();
	}
	
	for (USceneComponent* ChildComponent : SpawnPointGroupComponent->GetAttachChildren())
	{
		ULLL_MonsterSpawnPointComponent* SpawnPoint = Cast<ULLL_MonsterSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			SpawnPoints.Add(SpawnPoint);
		}
	}

	if (bSpawnByOwnerMonsterHealth)
	{
		SetOwner(OwnerMonster);
		OwnerMonster->CharacterDeadDelegate.AddDynamic(this, &ALLL_MonsterSpawner::OwnerMonsterDeadHandle);
		OwnerMonster->TakeDamageDelegate.AddDynamic(this, &ALLL_MonsterSpawner::OwnerMonsterDamagedHandle);
	}
}

void ALLL_MonsterSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!bSpawnByOwnerMonsterHealth && CurrentWave == 0)
	{
		const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor);
		if (IsValid(Player))
		{
			SpawnMonster();
			StartSpawnDelegate.Broadcast();
		}
	}
}

void ALLL_MonsterSpawner::SpawnMonster()
{
	CurrentWave++;
	
	CurrentGroup++;
	if (CurrentGroup > LastGroup)
	{
		CurrentGroup -= LastGroup;
	}

	for (const ULLL_MonsterSpawnPointComponent* SpawnPoint : SpawnPoints)
	{
		if (!IsValid(SpawnPoint))
		{
			continue;
		}

		int32 SpawnPointNum = SpawnPoint->GetNum();

		for (const FMonsterSpawnDataTable MonsterSpawnData : MonsterSpawnDataArray)
		{
			if (MonsterSpawnData.Group != CurrentGroup || MonsterSpawnData.SpawnPoint != SpawnPointNum || !IsValid(MonsterSpawnData.MonsterClass))
			{
				continue;
			}

			FTimerHandle MonsterSpawnTimerHandle;
			GetWorldTimerManager().SetTimer(MonsterSpawnTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
			{
				ALLL_MonsterBase* Monster = SpawnedMonster(MonsterSpawnData.MonsterClass, MonsterSpawnData.bIsElite, SpawnPoint->GetComponentTransform());
				if (IsValid(Monster))
				{
					Monster->CharacterDeadDelegate.AddDynamic(this, &ALLL_MonsterSpawner::MonsterDeadHandle);
					Monsters.Emplace(Monster);
						
	#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
					if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
					{
						if (DebugGameInstance->CheckMonsterSpawnDataDebug())
						{
							GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 스폰 (웨이브 : %d, 그룹 : %d, 스폰 포인트 : %d)"), *GetName(), CurrentWave, CurrentGroup, SpawnPointNum));
						}
					}
	#endif
				}
			}), MonsterSpawnerDataAsset->SpawnTimer, false);

			FTimerHandle SpawnParticleTimerHandle;
			GetWorldTimerManager().SetTimer(SpawnParticleTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, SpawnPoint]
			{
				AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MonsterSpawnerDataAsset->SpawnParticle, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation()));
			}), MonsterSpawnerDataAsset->SpawnParticleTimer, false);
		}
	}

	FModAudioComponent->Release();
	FModAudioComponent->SetEvent(MonsterSpawnerDataAsset->SpawnSoundEvent);
	FModAudioComponent->Play();
}

bool ALLL_MonsterSpawner::CheckNextWaveCanSpawnByOwnerMonsterHealth() const
{
	const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(OwnerMonster->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
	const float MaxHealth = MonsterAttributeSet->GetMaxHealth();
	const float CurrentHealth = MonsterAttributeSet->GetCurrentHealth();
	const float MaxShield = MonsterAttributeSet->GetMaxShield();
	const float CurrentShield = MonsterAttributeSet->GetCurrentShield();
	const float HealthRate = (CurrentHealth + CurrentShield) / (MaxHealth + MaxShield) * 100.0f;

	const float TriggerHealthRate = SpawnStartHealthRate - CurrentWave * HealthRateSpawnOffset;
	return HealthRate <= TriggerHealthRate && CurrentWave < MaxWave;
}

ALLL_MonsterBase* ALLL_MonsterSpawner::SpawnedMonster(const TSubclassOf<ALLL_MonsterBase>& MonsterClass, const bool IsElite, const FTransform& Transform) const
{
	ALLL_MonsterBase* Monster = GetWorld()->SpawnActorDeferred<ALLL_MonsterBase>(MonsterClass, Transform);
	if (!IsValid(Monster))
	{
		return nullptr;
	}
	Monster->SetIsElite(IsElite);
	Monster->FinishSpawning(Transform);

	return Monster;
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
			if (!bSpawnByOwnerMonsterHealth)
			{
				SpawnMonster();
			}
		}
		else
		{
			Destroy();
		}
	}
}

void ALLL_MonsterSpawner::OwnerMonsterDeadHandle(ALLL_BaseCharacter* BaseCharacter)
{
	while (CheckNextWaveCanSpawnByOwnerMonsterHealth())
	{
		SpawnMonster();
		
		if (CurrentWave == 1)
		{
			StartSpawnDelegate.Broadcast();
		}
	}
}

void ALLL_MonsterSpawner::OwnerMonsterDamagedHandle(bool IsDOT)
{
	while (CheckNextWaveCanSpawnByOwnerMonsterHealth())
	{
		SpawnMonster();
		
		if (CurrentWave == 1)
		{
			StartSpawnDelegate.Broadcast();
		}
	}
}
