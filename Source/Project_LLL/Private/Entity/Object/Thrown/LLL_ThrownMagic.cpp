// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownMagic.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ThrownMagicDataAsset.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Object/Thrown/LLL_MagicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownMagic::ALLL_ThrownMagic()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownMagicDataAsset>(PATH_THROWN_MAGIC_DATA, EAssertionLevel::Check);

	MagicAttributeSet = CreateDefaultSubobject<ULLL_MagicAttributeSet>(TEXT("StaffBasicMagicAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_MONSTER_ATTACK);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownMagic::BeginPlay()
{
	Super::BeginPlay();

	MagicDataAsset = Cast<ULLL_ThrownMagicDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = MagicAttributeSet;

	HitCollisionBox->SetBoxExtent(MagicDataAsset->HitCollisionSize);
}

void ALLL_ThrownMagic::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			HitCollisionBox->SetHiddenInGame(false);
		}
		else
		{
			HitCollisionBox->SetHiddenInGame(true);
		}
	}
#endif
}

void ALLL_ThrownMagic::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_ThrownMagic::Deactivate()
{
	Super::Deactivate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
