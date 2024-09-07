// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownFeather.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ThrownFeatherDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownFeatherAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownFeather::ALLL_ThrownFeather()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownFeatherDataAsset>(PATH_THROWN_FEATHER_DATA, EAssertionLevel::Check);
	
	ThrownFeatherAttributeSet = CreateDefaultSubobject<ULLL_ThrownFeatherAttributeSet>(TEXT("PlayerThrownFeatherAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownFeather::BeginPlay()
{
	Super::BeginPlay();

	ThrownFeatherDataAsset = Cast<ULLL_ThrownFeatherDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownFeatherAttributeSet;

	HitCollisionBox->SetBoxExtent(ThrownFeatherDataAsset->HitCollisionSize);

	ChaseFeatherThrowAngleOffset = ThrownFeatherDataAsset->ChaseFeatherThrowAngleOffset;
	ChaseFeatherThrowAngleRandomMultiply = ThrownFeatherDataAsset->ChaseFeatherThrowAngleRandomMultiply;
}

void ALLL_ThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
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

void ALLL_ThrownFeather::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_ThrownFeather::Deactivate()
{
	Super::Deactivate();
	
	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
