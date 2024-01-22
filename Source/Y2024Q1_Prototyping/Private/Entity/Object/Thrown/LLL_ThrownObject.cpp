// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownObject.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
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
	
	ProjectileMovement->Activate();
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;

	DelayedDestroy(3.0f);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Other))
	{
		if (ALLL_RangedMonster* RangedMonster = Cast<ALLL_RangedMonster>(GetOwner()))
		{
			const float OffencePower = RangedMonster->GetOffencePower();
			const FDamageEvent DamageEvent;
			Player->TakeDamage(OffencePower, DamageEvent, RangedMonster->GetController(), RangedMonster);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckMonsterAttackDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("투사체와 플레이어 충돌")));
				}
			}
#endif
			Destroy();
		}
	}
}
