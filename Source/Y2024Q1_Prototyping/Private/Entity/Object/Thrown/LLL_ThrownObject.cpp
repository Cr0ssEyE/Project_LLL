// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownObject.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	if (IsValid(BaseMesh))
	{
		BaseMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		BaseMesh->SetCollisionProfileName(CP_INTERACTION);
		CollisionBox->SetCollisionProfileName(CP_INTERACTION);
	}
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (ProjectileMovement)
	{
		ProjectileMovement->bShouldBounce = false;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
		ProjectileMovement->InitialSpeed = 0.0f;
		ProjectileMovement->MaxSpeed = Speed = 1000.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->Deactivate();
	}
}

void ALLL_ThrownObject::Throw(AActor* NewOwner)
{
	SetOwner(NewOwner);
	
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;

	DelayedDestroy(3.0f);
}
