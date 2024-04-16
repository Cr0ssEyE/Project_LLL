// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownObject.h"

#include "AbilitySystemComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/ThrownObject/LLL_ThrownObjectAttributeSet.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	if (IsValid(BaseMesh))
	{
		SetRootComponent(BaseMesh);
		BaseMesh->SetCollisionProfileName(CP_MONSTER_ATTACK);
	}
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		ProjectileMovementComponent->InitialSpeed = 0.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->Deactivate();
	}
}

void ALLL_ThrownObject::BeginPlay()
{
	Super::BeginPlay();

	ThrownObjectDataAsset = Cast<ULLL_ThrownObjectDataAsset>(BaseObjectDataAsset);
	
	ASC->AddSpawnedAttribute(ThrownObjectAttributeSet);
}

void ALLL_ThrownObject::Throw(AActor* NewOwner)
{
	SetOwner(NewOwner);

	Activate();
	ProjectileMovementComponent->MaxSpeed = ThrownObjectAttributeSet->GetThrowSpeed();
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	
	FTimerHandle HideTimerHandle;
	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ALLL_ThrownObject::Deactivate, ThrownObjectAttributeSet->GetHideTimer(), false);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Other))
	{
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
	}
	
	Deactivate();
}

void ALLL_ThrownObject::Activate()
{
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMovementComponent->Activate();
}

void ALLL_ThrownObject::Deactivate()
{
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
}
