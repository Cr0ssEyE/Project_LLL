// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownObject.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Ranged/LLL_RangedMonster.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Util/LLLConstructorHelper.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	BaseObjectDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_ThrownObjectDataAsset>(PATH_THROWN_OBJECT_DATA, EAssertionLevel::Check);
	ThrownObjectDataAsset = Cast<ULLL_ThrownObjectDataAsset>(BaseObjectDataAsset);
	
	if (IsValid(BaseMesh))
	{
		SetRootComponent(BaseMesh);
		BaseMesh->SetCollisionProfileName(CP_MONSTER_THROWN);
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

	Activate();
	ProjectileMovement->Velocity = GetActorForwardVector() * Speed;

	DelayedHide(3.0f);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Other))
	{
		if (ALLL_RangedMonster* RangedMonster = Cast<ALLL_RangedMonster>(GetOwner()))
		{
			// GE 기반으로 플레이어에게 데미지
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, 1.0, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckMonsterHitCheckDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("투사체와 플레이어 충돌")));
				}
			}
#endif

			Deactivate();
		}
	}
	else
	{
		Deactivate();
	}
}

void ALLL_ThrownObject::Activate()
{
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMovement->Activate();
}

void ALLL_ThrownObject::Deactivate()
{
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->Deactivate();
	SetActorHiddenInGame(true);
}
