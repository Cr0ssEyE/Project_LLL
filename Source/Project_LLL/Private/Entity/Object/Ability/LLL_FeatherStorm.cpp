// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_FeatherStorm.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_FeatherStormAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_FeatherStorm::ALLL_FeatherStorm()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_FeatherStormDataAsset>(PATH_FEATHER_STORM_DATA, EAssertionLevel::Check);

	FeatherStormAttributeSet = CreateDefaultSubobject<ULLL_FeatherStormAttributeSet>(TEXT("PlayerFeatherStormAttributeSet"));
}

void ALLL_FeatherStorm::BeginPlay()
{
	Super::BeginPlay();

	FeatherStormDataAsset = Cast<ULLL_FeatherStormDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = FeatherStormAttributeSet;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), OverlapCollisionSphere->GetScaledSphereRadius(), 16, FColor::Blue, false, AbilityObjectAttributeSet->GetDestroyTimer());
		}
	}
#endif
}

void ALLL_FeatherStorm::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	DamageToOverlapActor(OtherActor);
	GetWorldTimerManager().SetTimer(KeepDamageHandle, FTimerDelegate::CreateWeakLambda(this, [&, OtherActor]{
		DamageToOverlapActor(OtherActor);
	}), FeatherStormAttributeSet->GetDamageTimer(), true);
}

void ALLL_FeatherStorm::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	GetWorldTimerManager().ClearTimer(KeepDamageHandle);
}
