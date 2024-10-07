// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownBleeding.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_ThrownBleedingDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownBleedingAttributeSet.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownBleeding::ALLL_ThrownBleeding()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownBleedingDataAsset>(PATH_THROWN_BLEEDING_DATA, EAssertionLevel::Check);

	ThrownBleedingAttributeSet = CreateDefaultSubobject<ULLL_ThrownBleedingAttributeSet>(TEXT("ThrownBleedingAttributeSet"));
	
	OverlapCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Collision"));
	OverlapCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT_OVERLAP);
	SetRootComponent(OverlapCollisionBox);
}

void ALLL_ThrownBleeding::BeginPlay()
{
	Super::BeginPlay();

	ThrownBleedingDataAsset = Cast<ULLL_ThrownBleedingDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownBleedingAttributeSet;

	OverlapCollisionBox->SetBoxExtent(ThrownBleedingDataAsset->HitCollisionSize);
	// 이펙트가 나오기 전까지 디버그 유지
	OverlapCollisionBox->SetHiddenInGame(false);
}

void ALLL_ThrownBleeding::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			//OverlapCollisionBox->SetHiddenInGame(false);
		}
		else
		{
			//OverlapCollisionBox->SetHiddenInGame(true);
		}
	}
#endif
}

void ALLL_ThrownBleeding::Activate()
{
	Super::Activate();

	OverlapCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->UpdatedComponent = OverlapCollisionBox;
}

void ALLL_ThrownBleeding::Deactivate()
{
	Super::Deactivate();
	
	OverlapCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamagedMonsters.Empty();
}

void ALLL_ThrownBleeding::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	Super::Throw(NewOwner, NewTarget, InSpeed, Straight, InKnockBackPower);

	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
	{
		OffencePower = AbilityData->AbilityValue1 / static_cast<uint32>(AbilityData->Value1Type);
		OffencePower = FLLL_AbilityDataHelper::CalculateOffencePower(OffencePower, Player);

		BleedingOffencePower = AbilityData->AbilityValue2 / static_cast<uint32>(AbilityData->Value2Type);
		BleedingOffencePower = FLLL_AbilityDataHelper::CalculateOffencePower(BleedingOffencePower, Player);
	}
}

void ALLL_ThrownBleeding::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor))
	{
		if (DamagedMonsters.Contains(Monster))
		{
			return;
		}

		DamagedMonsters.Emplace(Monster);
		Monster->SetBleedingStack(Monster->GetBleedingStack() + 4);

		if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			EffectContextHandle.AddInstigator(Player, this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownBleedingDataAsset->BleedingDamageEffect, AbilityLevel, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, BleedingOffencePower);
				FLLL_AbilityDataHelper::SetBleedingPeriodValue(Player, CastChecked<ULLL_ExtendedGameplayEffect>(ThrownBleedingDataAsset->BleedingDamageEffect.GetDefaultObject()));
				if (!FLLL_AbilityDataHelper::CheckBleedingExplosion(Player, Monster, this))
				{
					ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Monster->GetAbilitySystemComponent());
				}
			}
		}
		
		DamageTo(OtherActor);
	}
}
