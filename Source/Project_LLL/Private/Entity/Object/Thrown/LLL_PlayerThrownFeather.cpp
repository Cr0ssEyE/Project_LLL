// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerThrownFeather.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_PlayerThrownFeatherDataAsset.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_PlayerThrownFeatherAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerThrownFeather::ALLL_PlayerThrownFeather()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerThrownFeatherDataAsset>(PATH_PLAYER_THROWN_FEATHER_DATA, EAssertionLevel::Check);
	
	PlayerThrownFeatherAttributeSet = CreateDefaultSubobject<ULLL_PlayerThrownFeatherAttributeSet>(TEXT("PlayerThrownFeatherAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);
	SetRootComponent(HitCollisionBox);
}

void ALLL_PlayerThrownFeather::BeginPlay()
{
	Super::BeginPlay();

	PlayerThrownFeatherDataAsset = Cast<ULLL_PlayerThrownFeatherDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = PlayerThrownFeatherAttributeSet;

	HitCollisionBox->SetBoxExtent(PlayerThrownFeatherDataAsset->HitCollisionSize);
	
	CurveSize = PlayerThrownFeatherAttributeSet->GetCurveSize();
	CurveSpeed = 1.0f / CurveSize;
}

void ALLL_PlayerThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsActivated())
	{
		const FVector Direction = Target->GetActorLocation() - GetActorLocation();
		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaSeconds, CurveSpeed));
		CurveSpeed += 1.0f / CurveSize;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug())
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

void ALLL_PlayerThrownFeather::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_PlayerThrownFeather::Deactivate()
{
	Super::Deactivate();
	
	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CurveSpeed = 1.0f / CurveSize;
}
