// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_PlayerThreateningDeath.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_PlayerThreateningDeathAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Util/LLL_MathHelper.h"

ALLL_PlayerThreateningDeath::ALLL_PlayerThreateningDeath()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerThreateningDeathDataAsset>(PATH_PLAYER_THREATENING_DEATH_DATA, EAssertionLevel::Check);

	PlayerThreateningDeathAttributeSet = CreateDefaultSubobject<ULLL_PlayerThreateningDeathAttributeSet>(TEXT("PlayerThreateningDeathAttributeSet"));
}

void ALLL_PlayerThreateningDeath::BeginPlay()
{
	Super::BeginPlay();

	PlayerThreateningDeathDataAsset = Cast<ULLL_PlayerThreateningDeathDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = PlayerThreateningDeathAttributeSet;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerDashDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), OverlapCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, PlayerThreateningDeathAttributeSet->GetDestroyTimer());
		}
	}
#endif
}

void ALLL_PlayerThreateningDeath::NotifyActorBeginOverlap(AActor* OtherActor)
{
	ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(OtherActor);
	if (!KnockBackActor)
	{
		return;
	}

	const FVector AvatarLocation = GetActorLocation();
	const FVector LaunchDirection = (OtherActor->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
	const float KnockBackPower = (AbilityData->AbilityValue + AbilityData->ChangeValue * AbilityLevel) / static_cast<uint32>(AbilityData->AbilityValueType) * 100.0f;
	FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, KnockBackPower);

	UE_LOG(LogTemp, Log, TEXT("넉백 : %f"), KnockBackPower)
	KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
}
