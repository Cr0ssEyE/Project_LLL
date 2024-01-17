// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerWeaponComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "DataAsset/LLL_WeaponBaseDataAsset.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/Character.h"
#include "Util/LLL_MathHelper.h"


// Sets default values for this component's properties
ULLL_PlayerWeaponComponent::ULLL_PlayerWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsHitCheckOnGoing = false;
}

void ULLL_PlayerWeaponComponent::SetupWeaponInfo(const ULLL_WeaponBaseDataAsset* WeaponDataAsset)
{
	Damage = WeaponDataAsset->WeaponBaseDamage;
	MaxAttackActionCount = WeaponDataAsset->WeaponAttackActionCount;
	AttackActionProperties = WeaponDataAsset->WeaponActionProperties;
}

void ULLL_PlayerWeaponComponent::StartMeleeWeaponHitCheck(uint32 CurrentActionCount)
{
	bIsHitCheckOnGoing = true;
	CurrentAttackActionCount = CurrentActionCount;
	MeleeWeaponHitCheck();
}

void ULLL_PlayerWeaponComponent::StopMeleeWeaponHitCheck()
{
	bIsHitCheckOnGoing = false;
	CurrentAttackActionCount = 0;
	DamagedActors.Empty();
}

void ULLL_PlayerWeaponComponent::MeleeWeaponHitCheck()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if(!IsValid(OwnerCharacter))
	{
		return;
	}
	
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(NAME_None, false, OwnerCharacter);
	if(!DamagedActors.IsEmpty())
	{
		Params.AddIgnoredActors(DamagedActors);
	}

	float FrontLocation = AttackActionProperties[CurrentAttackActionCount].ActionHitBoxLocation;
	FVector HitBoxLocation = FLLL_MathHelper::CalculateComponentFrontPoint(OwnerCharacter->GetRootComponent(), FrontLocation);
	
	bool HitEntity = GetWorld()->SweepMultiByChannel(
		HitResults,
		HitBoxLocation,
		HitBoxLocation,
		FQuat::Identity,
		ECC_ENEMY_ONLY,
		FCollisionShape::MakeBox(AttackActionProperties[CurrentAttackActionCount].ActionHitBoxSize),
		Params);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug())
		{
			DrawDebugBox(GetWorld(), HitBoxLocation, AttackActionProperties[CurrentAttackActionCount].ActionHitBoxSize, FColor::Red, false, 0.5f);
		}
	}
#endif
	
	if(HitEntity)
	{
		FDamageEvent DamageEvent;
		for (auto HitResult : HitResults)
		{
			AActor* HitActor = HitResult.GetActor();
			DamagedActors.Add(HitActor);

			float DamageMultiply = AttackActionProperties[CurrentAttackActionCount].ActionDamageMultiplyValue;
			uint32 DamageResult = FLLL_MathHelper::CalculatePlayerWeaponDamage(1, Damage, DamageMultiply);
			HitActor->TakeDamage(DamageResult, DamageEvent, OwnerCharacter->GetController(), OwnerCharacter);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckPlayerAttackDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("플레이어 공격 데미지 처리 실행. 입힌 데미지: %d"), DamageResult));
				}
			}
#endif
			
			if(ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(HitActor))
			{
				//TODO: 넉백 구현하기
			}
		}
	}
}


