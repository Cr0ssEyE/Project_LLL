// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_Monster_Id.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDashAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_SwordDash::ALLL_SwordDash()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_SwordDashDataAsset>(PATH_SWORD_DASH_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_SwordDashAIController::StaticClass();

	Id = ID_SWORD_DASH;

	DashDamageRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DashDamageRangeBox->SetCollisionProfileName(CP_INTERACTION);
	DashDamageRangeBox->SetupAttachment(RootComponent);

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	SwordMeshComponent->SetupAttachment(RootComponent);

	ShoulderGuardMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShoulderGuard"));
	ShoulderGuardMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	ShoulderGuardMeshComponent->SetupAttachment(RootComponent);
}

void ALLL_SwordDash::BeginPlay()
{
	Super::BeginPlay();

	SwordDashDataAsset = Cast<ULLL_SwordDashDataAsset>(MeleeMonsterDataAsset);
	
	SwordMeshComponent->SetStaticMesh(SwordDashDataAsset->SwordMesh);
	SwordMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SwordDashDataAsset->SwordAttachSocketName);
	SwordMeshComponent->SetRelativeTransform(SwordDashDataAsset->SwordTransform);

	ShoulderGuardMeshComponent->SetStaticMesh(SwordDashDataAsset->ShoulderGuardMesh);
	ShoulderGuardMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SwordDashDataAsset->ShoulderGuardAttachSocketName);
	ShoulderGuardMeshComponent->SetRelativeTransform(SwordDashDataAsset->ShoulderGuardTransform);
}

void ALLL_SwordDash::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			if (bIsDashing)
			{
				const FVector Location = DashDamageRangeBox->GetComponentLocation();
				const FVector Extent = DashDamageRangeBox->GetScaledBoxExtent();
				const FQuat Quat = DashDamageRangeBox->GetComponentQuat();
				DrawDebugBox(GetWorld(), Location, Extent, Quat, FColor::Blue, false, 2.0f);
			}
		}
	}
#endif
}

void ALLL_SwordDash::InitAttributeSet()
{
	Super::InitAttributeSet();
	
	DashDamageRangeBox->SetBoxExtent(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), MonsterAttributeSet->GetMonsterData4(), MonsterAttributeSet->GetMonsterData4()));
}

void ALLL_SwordDash::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsDashing)
	{
		if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(SwordDashDataAsset->DashDamageEffect, Level, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (DebugGameInstance->CheckMonsterHitCheckDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("대시 피격")));
				}
			}
#endif
		}
	}
}

void ALLL_SwordDash::Dash() const
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_SWORD_DASH_DASH)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 대시 수행"), *GetName()));
			}
		}
#endif
	}
}
