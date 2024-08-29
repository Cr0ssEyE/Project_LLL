// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_Shockwave.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_ShockwaveAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_Shockwave::ALLL_Shockwave()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ShockwavetDataAsset>(PATH_SHOCKWAVE_DATA, EAssertionLevel::Check);

	ShockwaveAttributeSet = CreateDefaultSubobject<ULLL_ShockwaveAttributeSet>(TEXT("ShockwaveAttributeSet"));

	OverlapCollisionSphere->SetCollisionProfileName(CP_MONSTER_ABILITY_OBJECT);
}

void ALLL_Shockwave::BeginPlay()
{
	Super::BeginPlay();

	ShockwaveDataAsset = Cast<ULLL_ShockwavetDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = ShockwaveAttributeSet;

	FVector Location = GetActorLocation();
	Location.Z = 1.0f;
	SetActorLocation(Location);
}

void ALLL_Shockwave::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			DrawDebugCircle(GetWorld(), GetActorLocation(), OverlapCollisionSphere->GetScaledSphereRadius(), 16, FColor::Yellow, false, 0, 0, 0, FVector::RightVector, FVector::ForwardVector);
			DrawDebugCircle(GetWorld(), GetActorLocation(), OverlapCollisionSphere->GetScaledSphereRadius() - ShockwaveAttributeSet->GetThickness() * 2, 16, FColor::Yellow, false, 0, 0, 0, FVector::RightVector, FVector::ForwardVector);
		}
	}
#endif
	
	FVector Scale = GetActorScale3D();
	Scale += FVector(ShockwaveAttributeSet->GetSpeed() * DeltaSeconds);
	SetActorScale3D(Scale);
}

void ALLL_Shockwave::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor))
	{
		if (CastChecked<ULLL_PlayerAnimInstance>(Player->GetCharacterAnimInstance())->IsDashing())
		{
			return;
		}
	}
	
	if (GetDistanceTo(OtherActor) < OverlapCollisionSphere->GetScaledSphereRadius() - ShockwaveAttributeSet->GetThickness())
	{
		return;
	}
	
	DamageToOverlapActor(OtherActor);
}
