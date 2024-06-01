// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownFeather.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ThrownFeatherDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownFeatherAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownFeather::ALLL_ThrownFeather()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownFeatherDataAsset>(PATH_THROWN_FEATHER_DATA, EAssertionLevel::Check);
	
	ThrownFeatherAttributeSet = CreateDefaultSubobject<ULLL_ThrownFeatherAttributeSet>(TEXT("PlayerThrownFeatherAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownFeather::BeginPlay()
{
	Super::BeginPlay();

	ThrownFeatherDataAsset = Cast<ULLL_ThrownFeatherDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownFeatherAttributeSet;

	HitCollisionBox->SetBoxExtent(ThrownFeatherDataAsset->HitCollisionSize);
	
	CurveSize = ThrownFeatherAttributeSet->GetCurveSize();
	CurveSpeed = 1.0f / CurveSize;
}

void ALLL_ThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsActivated())
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

void ALLL_ThrownFeather::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_ThrownFeather::Deactivate()
{
	Super::Deactivate();
	
	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CurveSpeed = 1.0f / CurveSize;

	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter) && TargetCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_ThrownFeather::TargetDeadHandle))
	{
		TargetCharacter->CharacterDeadDelegate.RemoveDynamic(this, &ALLL_ThrownFeather::TargetDeadHandle);
	}
	bTargetIsDead = false;
}

void ALLL_ThrownFeather::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed)
{
	Super::Throw(NewOwner, NewTarget, InSpeed);

	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter))
	{
		TargetCapsuleRadius = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (!TargetCharacter->CheckCharacterIsDead())
		{
			TargetCharacter->CharacterDeadDelegate.AddDynamic(this, &ALLL_ThrownFeather::TargetDeadHandle);
		}
		else
		{
			TargetDeadLocation = Target->GetActorLocation();
			bTargetIsDead = true;
		}
	}
}

void ALLL_ThrownFeather::TargetDeadHandle(ALLL_BaseCharacter* Character)
{
	TargetDeadLocation = Character->GetActorLocation();
	bTargetIsDead = true;
}
