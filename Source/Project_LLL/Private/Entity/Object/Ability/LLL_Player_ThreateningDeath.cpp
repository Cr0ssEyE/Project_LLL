// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_Player_ThreateningDeath.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_ThreateningDeathAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Util/LLL_MathHelper.h"

ALLL_Player_ThreateningDeath::ALLL_Player_ThreateningDeath()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerThreateningDeathDataAsset>(PATH_PLAYER_THREATENING_DEATH_DATA, EAssertionLevel::Check);
	ThreateningDeathDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerThreateningDeathDataAsset>(PATH_PLAYER_THREATENING_DEATH_DATA, EAssertionLevel::Check);

	AbilityObjectAttributeSet = CreateDefaultSubobject<ULLL_ThreateningDeathAttributeSet>(TEXT("PlayerThreateningDeathAttributeSet"));
}

void ALLL_Player_ThreateningDeath::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	const FVector AvatarLocation = GetActorLocation();
	
	if (ILLL_KnockBackInterface* KnockBackActor = CastChecked<ILLL_KnockBackInterface>(OtherActor))
	{
		const FVector LaunchDirection = (OtherActor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, ThreateningDeathDataAsset->KnockBackPower);
			
		KnockBackActor->AddKnockBackVelocity(LaunchVelocity, ThreateningDeathDataAsset->KnockBackPower);
	}
}

void ALLL_Player_ThreateningDeath::BeginPlay()
{
	Super::BeginPlay();
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ThreateningDeathDataAsset->Particle, GetActorLocation(), GetActorRotation());

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerDashDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), OverlapCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, AbilityObjectAttributeSet->GetDestroyTimer());
		}
	}
#endif

	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		Destroy();
	}), 0.1, false);
}
