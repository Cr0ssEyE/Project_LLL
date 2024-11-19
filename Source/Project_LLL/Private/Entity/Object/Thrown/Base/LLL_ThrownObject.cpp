// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"

#include "AbilitySystemComponent.h"
#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_MathHelper.h"

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

	FModAudioComponent->SetEvent(ThrownObjectDataAsset->FModEvent);
}

void ALLL_ThrownObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsActivated() && !bIsStraight)
	{
		if (bTargetIsDead && FVector::Distance(GetActorLocation(), TargetDeadLocation) <= TargetCapsuleRadius)
		{
			Deactivate();
		}

		const FVector TargetLocation = bTargetIsDead ? TargetDeadLocation : Target->GetActorLocation();
		const FVector Direction = TargetLocation - GetActorLocation();
		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaSeconds, CurveSpeed));
		CurveSpeed += 1.0f / CurveSize;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	}
}

void ALLL_ThrownObject::Activate()
{
	bIsActivated = true;

	AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->Activate();
	SetActorHiddenInGame(false);
	AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));

	CurveSize = ThrownObjectAttributeSet->GetCurveSize();
	CurveSpeed = 1.0f / ThrownObjectAttributeSet->GetCurveSize();
}

void ALLL_ThrownObject::Deactivate()
{
	bIsActivated = false;
	
	FModAudioComponent->Stop();
	FModAudioComponent->Release();
	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
		TempNiagaraComponent->Deactivate();
		TempNiagaraComponent->SetVisibility(false);
		NiagaraComponents.Remove(TempNiagaraComponent);
	}
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
	
	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter) && TargetCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_ThrownObject::TargetDeadHandle))
	{
		TargetCharacter->CharacterDeadDelegate.RemoveDynamic(this, &ALLL_ThrownObject::TargetDeadHandle);
	}
	bTargetIsDead = false;

	GetWorldTimerManager().ClearTimer(HideTimerHandle);
}

void ALLL_ThrownObject::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	SetOwner(NewOwner);
	Target = NewTarget;

	ProjectileMovementComponent->MaxSpeed = InSpeed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	
	FModAudioComponent->SetPitch(Owner->CustomTimeDilation);
	FModAudioComponent->Play();

	bIsStraight = Straight;
	KnockBackPower = InKnockBackPower;
	
	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
	{
		if (InKnockBackPower > 0)
		{
			KnockBackPower = FLLL_AbilityDataHelper::CalculateKnockBackPower(KnockBackPower, Player);
		}
	}

	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter))
	{
		TargetCapsuleRadius = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (!TargetCharacter->CheckCharacterIsDead() && !TargetCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_ThrownObject::TargetDeadHandle))
		{
			TargetCharacter->CharacterDeadDelegate.AddDynamic(this, &ALLL_ThrownObject::TargetDeadHandle);
		}
		else
		{
			TargetDeadLocation = Target->GetActorLocation();
			bTargetIsDead = true;
		}
	}

	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ALLL_ThrownObject::Deactivate, ThrownObjectAttributeSet->GetHideTimer(), false);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	DamageTo(Other);
	
	const FVector Direction = (Other->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal2D();
	KnockBackTo(Direction, Other);
	
	Deactivate();
}

void ALLL_ThrownObject::DamageTo(AActor* OtherActor)
{
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (AbilitySystemInterface && OtherActor != GetOwner())
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(GetOwner(), this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, AbilityLevel, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
		}
	}
}

void ALLL_ThrownObject::KnockBackTo(const FVector& Direction, AActor* OtherActor) const
{
	ILLL_KnockBackInterface* KnockBackInterface = Cast<ILLL_KnockBackInterface>(OtherActor);
	if (KnockBackInterface && KnockBackPower != 0.0f)
	{
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(Direction, KnockBackPower);
		KnockBackInterface->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
	}
}

void ALLL_ThrownObject::TargetDeadHandle(ALLL_BaseCharacter* Character)
{
	TargetDeadLocation = Character->GetActorLocation();
	bTargetIsDead = true;
}
