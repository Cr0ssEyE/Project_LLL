// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownObject.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	if (IsValid(BaseMesh))
	{
		BaseMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		BaseMesh->SetCollisionObjectType(ECC_PLAYER_ONLY);
		CollisionBox->SetCollisionObjectType(ECC_PLAYER_ONLY);
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

	BaseMesh->OnComponentHit.AddDynamic(this, &ALLL_ThrownObject::HandleHit);
	
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;

	DelayedDestroy(3.0f);
}

void ALLL_ThrownObject::HandleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor))
	{
		if (ALLL_RangedMonster* RangedMonster = Cast<ALLL_RangedMonster>(GetOwner()))
		{
			const float OffencePower = RangedMonster->GetOffencePower();
			const FDamageEvent DamageEvent;
			Player->TakeDamage(OffencePower, DamageEvent, RangedMonster->GetController(), RangedMonster);
			
			Destroy();
		}
	}
}
