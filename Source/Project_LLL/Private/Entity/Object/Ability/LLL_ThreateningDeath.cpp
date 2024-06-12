// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_ThreateningDeath.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_AbilityObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Object/Ability/LLL_ThreateningDeathAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Util/LLL_MathHelper.h"

ALLL_ThreateningDeath::ALLL_ThreateningDeath()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThreateningDeathDataAsset>(PATH_THREATENING_DEATH_DATA, EAssertionLevel::Check);

	ThreateningDeathAttributeSet = CreateDefaultSubobject<ULLL_ThreateningDeathAttributeSet>(TEXT("PlayerThreateningDeathAttributeSet"));
}

void ALLL_ThreateningDeath::BeginPlay()
{
	Super::BeginPlay();

	ThreateningDeathDataAsset = Cast<ULLL_ThreateningDeathDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = ThreateningDeathAttributeSet;

	FTimerHandle ActivateKnockBackTimer;
	GetWorldTimerManager().SetTimer(ActivateKnockBackTimer, FTimerDelegate::CreateWeakLambda(this, [&]{
		SetActorEnableCollision(false);
	}), 0.01f, false);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerDashDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), OverlapCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, ThreateningDeathAttributeSet->GetDestroyTimer());
		}
	}
#endif
}

void ALLL_ThreateningDeath::NotifyActorBeginOverlap(AActor* OtherActor)
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
