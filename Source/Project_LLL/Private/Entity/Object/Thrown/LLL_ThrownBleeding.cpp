// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownBleeding.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ThrownBleedingDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownBleedingAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownBleeding::ALLL_ThrownBleeding()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownBleedingDataAsset>(PATH_THROWN_BLEEDING_DATA, EAssertionLevel::Check);

	ThrownBleedingAttributeSet = CreateDefaultSubobject<ULLL_ThrownBleedingAttributeSet>(TEXT("ThrownBleedingAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownBleeding::BeginPlay()
{
	Super::BeginPlay();

	ThrownBleedingDataAsset = Cast<ULLL_ThrownBleedingDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownBleedingAttributeSet;

	HitCollisionBox->SetBoxExtent(ThrownBleedingDataAsset->HitCollisionSize);
}

void ALLL_ThrownBleeding::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
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

void ALLL_ThrownBleeding::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->UpdatedComponent = HitCollisionBox;
}

void ALLL_ThrownBleeding::Deactivate()
{
	Super::Deactivate();
	
	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALLL_ThrownBleeding::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	Super::Throw(NewOwner, NewTarget, InSpeed, Straight, InKnockBackPower);

	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		OffencePower = AbilityData->AbilityValue2 / static_cast<uint32>(AbilityData->Value2Type);
		OffencePower *= PlayerAttributeSet->GetAllOffencePowerRate();
		OffencePower += PlayerAttributeSet->GetAllOffencePowerPlus();
	}
}
