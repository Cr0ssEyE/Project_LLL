// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/PlayerFeather/LLL_PlayerThrownFeather.h"

#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_PlayerThrownFeatherDataAsset.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/PlayerThrownFeather/LLL_PlayerThrownFeatherAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerThrownFeather::ALLL_PlayerThrownFeather()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerThrownFeatherDataAsset>(PATH_PLAYER_THROWN_FEATHER_DATA, EAssertionLevel::Check);
	
	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_PlayerThrownFeatherAttributeSet>(TEXT("PlayerThrownFeatherAttributeSet"));
	
	BaseMesh->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);

	CurveSize = 10.0f;
	CurrentCurveSize = 0.001f / CurveSize;
}

void ALLL_PlayerThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector Direction = Target->GetActorLocation() - GetActorLocation();
	Direction.Z = 0.0f;
	const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaSeconds, CurrentCurveSize * ProjectileMovementComponent->MaxSpeed));
	CurrentCurveSize += 0.001f / CurveSize;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
}

void ALLL_PlayerThrownFeather::Deactivate()
{
	CurrentCurveSize = 0.001f / CurveSize;
	
	Super::Deactivate();
}
