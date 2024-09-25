// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_Shockwave.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GraphicParameterNames.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
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

	FVector Scale = GetActorScale3D();
	Scale += FVector(ShockwaveAttributeSet->GetSpeed() * DeltaSeconds);
	SetActorScale3D(Scale);

	NiagaraComponents[0]->SetFloatParameter(NS_WAVE_RADIUS, GetActorScale3D().X / 75.0f + 0.025f);

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
}

void ALLL_Shockwave::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bPlayerHit)
	{
		return;
	}
	
	if (GetDistanceTo(OtherActor) < OverlapCollisionSphere->GetScaledSphereRadius() - ShockwaveAttributeSet->GetThickness())
	{
		return;
	}

	if (const ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(GetOwner()))
	{
		const UAbilitySystemComponent* ManOfStrengthASC = ManOfStrength->GetAbilitySystemComponent();
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrengthASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		OffencePower = MonsterAttributeSet->GetManOfStrengthShockwaveOffencePower();
		
		DamageTo(OtherActor);

		if (Cast<ALLL_PlayerBase>(OtherActor))
		{
			bPlayerHit = true;
		}
	}
}
