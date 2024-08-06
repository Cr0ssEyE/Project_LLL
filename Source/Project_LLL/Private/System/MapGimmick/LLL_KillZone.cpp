// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_KillZone.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GeneralConstants.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "System/MapGimmick/LLL_MapGimmick.h"
#include "System/MonsterSpawner/LLL_MonsterSpawner.h"
#include "System/MonsterSpawner/LLL_MonsterSpawnPointComponent.h"

// Sets default values
ALLL_KillZone::ALLL_KillZone()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("DeadZone"));
	Trigger->SetBoxExtent(FVector(BIG_SCALE_SCALAR * 2.f, BIG_SCALE_SCALAR * 2.f, MIDDLE_SCALE_SCALAR));
	Trigger->SetCollisionProfileName(CP_OVERLAP_ALL);
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALLL_KillZone::OnKillTriggerBeginOverlap);
}

// Called when the game starts or when spawned
void ALLL_KillZone::BeginPlay()
{
	Super::BeginPlay();
	AddActorLocalOffset(FVector(0.f, 0.f, -Trigger->GetScaledBoxExtent().Z));	
}

void ALLL_KillZone::OnKillTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if (IsValid(Monster))
	{
		Monster->Dead();
	}
	else if(ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor))
	{
		for (auto Actor : GetWorld()->GetCurrentLevel()->Actors)
		{
			ALLL_MapGimmick* MapGimmick = Cast<ALLL_MapGimmick>(Actor);
			if(!IsValid(MapGimmick))
			{
				continue;
			}

			float RespawnPointDistance = UE_DOUBLE_BIG_NUMBER;
			FVector ReSpawnPointLocation;

			if (!IsValid(MapGimmick->GetMonsterSpawner()))
			{
				PlayerCharacter->SetActorLocation(FVector(0.f, 0.f, PlayerCharacter->GetMesh()->GetRelativeLocation().Z));
			}

			if (MapGimmick->GetMonsterSpawner()->GetSpawnPoints().IsEmpty())
			{
				ensure(false);
				PlayerCharacter->SetActorLocation(FVector::Zero());
				return;
			}
			
			for (auto SpawnPoint : MapGimmick->GetMonsterSpawner()->GetSpawnPoints())
			{
				float PointDistance = FVector::Distance(PlayerCharacter->GetActorLocation(), SpawnPoint->GetComponentLocation());
				if(RespawnPointDistance > PointDistance)
				{
					RespawnPointDistance = PointDistance;
					ReSpawnPointLocation = SpawnPoint->GetComponentLocation();
					if (RespawnPointDistance < 100.f)
					{
						break;
					}
				}
			}
			ReSpawnPointLocation.Z -= PlayerCharacter->GetMesh()->GetRelativeLocation().Z;
			PlayerCharacter->SetActorLocation(ReSpawnPointLocation);
			break;
		}
	}
} 

