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
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Character/Monster/Base/LLL_MonsterAttributeSet.h"
#include "GAS/Attribute/Character/Monster/MeleeMonster/SwordDash/LLL_SwordDashAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_SwordDash::ALLL_SwordDash()
{
	SwordDashAttributeSet = CreateDefaultSubobject<ULLL_SwordDashAttributeSet>(TEXT("SwordDashAttributeSet"));
	
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_SwordDashDataAsset>(PATH_SWORD_DASH_DATA, EAssertionLevel::Check);
	AIControllerClass = ALLL_SwordDashAIController::StaticClass();

	Id = ID_SWORD_DASH;

	DashDamageRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DashDamageRangeBox->SetCollisionProfileName(CP_INTERACTION);
	DashDamageRangeBox->SetupAttachment(RootComponent);

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SwordMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	SwordMeshComponent->SetupAttachment(RootComponent);
}

void ALLL_SwordDash::BeginPlay()
{
	Super::BeginPlay();

	SwordDashDataAsset = Cast<ULLL_SwordDashDataAsset>(MeleeMonsterDataAsset);

	//CastChecked<ULLL_MonsterAttributeSet>(CharacterAttributeSet)->OnLoadMaxHealthDelegate.AddDynamic();
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		DashDamageRangeBox->SetBoxExtent(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), SwordDashAttributeSet->GetDashDamageRange(), SwordDashAttributeSet->GetDashDamageRange()));
	}));

	SwordMeshComponent->SetStaticMesh(SwordDashDataAsset->SwordMesh);
	SwordMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SwordDashDataAsset->SwordAttachSocketName);
	SwordMeshComponent->SetRelativeLocation(SwordDashDataAsset->SwordLocation);
	SwordMeshComponent->SetRelativeRotation(SwordDashDataAsset->SwordRotation);
	SwordMeshComponent->SetRelativeScale3D(SwordDashDataAsset->SwordScale);
}

void ALLL_SwordDash::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckMonsterAttackDebug())
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

void ALLL_SwordDash::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (bIsDashing)
	{
		if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(SwordDashDataAsset->DashDamageEffect, 1.0, EffectContextHandle);
			if(EffectSpecHandle.IsValid())
			{
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (ProtoGameInstance->CheckMonsterHitCheckDebug())
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
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 대시 수행"), *GetName()));
			}
		}
#endif
	}
}
