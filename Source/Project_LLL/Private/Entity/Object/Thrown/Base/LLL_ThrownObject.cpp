// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"

#include "AbilitySystemComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	if (IsValid(BaseMesh))
	{
		SetRootComponent(BaseMesh);
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

void ALLL_ThrownObject::Throw(AActor* NewOwner, const AActor* NewTarget)
{
	SetOwner(NewOwner);
	Target = NewTarget;

	ProjectileMovementComponent->MaxSpeed = ThrownObjectAttributeSet->GetThrowSpeed();
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	
	FTimerHandle HideTimerHandle;
	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ALLL_ThrownObject::Deactivate, ThrownObjectAttributeSet->GetHideTimer(), false);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Other))
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
		}
	}
	
	Deactivate();
}

void ALLL_ThrownObject::Activate()
{
	ILLL_ObjectPoolingObjectInterface::Activate();
	
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->Activate();
	SetActorHiddenInGame(false);
}

void ALLL_ThrownObject::Deactivate()
{
	ILLL_ObjectPoolingObjectInterface::Deactivate();
	
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
}
