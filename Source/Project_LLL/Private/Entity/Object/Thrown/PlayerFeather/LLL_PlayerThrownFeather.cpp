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

	CurveSize = 3.0f; // 추후 어트리뷰트 또는 데이터화 필요
	CurrentCurveSize = 1.0f / CurveSize;
}

void ALLL_PlayerThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsHidden())
	{
		FVector Direction = Target->GetActorLocation() - GetActorLocation();
		Direction.Z = 0.0f;
		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaSeconds, CurrentCurveSize));
		CurrentCurveSize += 1.0f / CurveSize;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	}
}

void ALLL_PlayerThrownFeather::Deactivate()
{
	Super::Deactivate();
	
	CurrentCurveSize = 1.0f / CurveSize;
}
