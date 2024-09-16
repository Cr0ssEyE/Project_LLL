// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_Shockwave.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
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
	
	if (GetDistanceTo(OtherActor) < OverlapCollisionSphere->GetScaledSphereRadius() - ShockwaveAttributeSet->GetThickness())
	{
		return;
	}

	if (const ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(GetOwner()))
	{
		const UAbilitySystemComponent* ManOfStrengthASC = ManOfStrength->GetAbilitySystemComponent();
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrengthASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		OffencePower = MonsterAttributeSet->GetManOfStrengthShockwaveOffencePower();
		DamageToOverlapActor(OtherActor);
	}
}
