// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MonsterSpawner/LLL_Debug_BossSpawner.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Util/LLL_ConstructorHelper.h"

class ALLL_PlayerBase;

ALLL_Debug_BossSpawner::ALLL_Debug_BossSpawner()
{
	BossClass = FLLL_ConstructorHelper::FindAndGetClass<ALLL_ManOfStrength>(PATH_MAN_OF_STRENGTH_BP, EAssertionLevel::Check);
	DetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DetectBox->SetCollisionProfileName(CP_INTERACTION);
	Boss = nullptr;
	SetRootComponent(DetectBox);
}

void ALLL_Debug_BossSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ALLL_SystemBase::NotifyActorBeginOverlap(OtherActor);
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor);
	if (IsValid(Player))
	{
		if (!IsValid(Boss))
		{
			Boss = GetWorld()->SpawnActor<ALLL_ManOfStrength>(BossClass, GetActorLocation(), GetActorRotation());
		}
	}
}

