// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_Monster_Id.h"
#include "DataAsset/LLL_ManOfStrengthDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Enumeration/LLL_BossMonsterPatternEnumHelper.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ManOfStrength::ALLL_ManOfStrength()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ManOfStrengthDataAsset>(PATH_MAN_OF_STRENGTH_DATA, EAssertionLevel::Check);

	Id = ID_MAN_OF_STRENGTH;
	
	DashDamageRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detect"));
	DashDamageRangeBox->SetCollisionProfileName(CP_INTERACTION);
	DashDamageRangeBox->SetupAttachment(RootComponent);
}

void ALLL_ManOfStrength::BeginPlay()
{
	Super::BeginPlay();

	ManOfStrengthDataAsset = Cast<ULLL_ManOfStrengthDataAsset>(BossMonsterDataAsset);
}

void ALLL_ManOfStrength::Tick(float DeltaSeconds)
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
				DrawDebugBox(GetWorld(), Location, Extent, Quat, FColor::Blue, false, 0.1f);
			}
		}
	}
#endif
}

void ALLL_ManOfStrength::InitAttributeSet()
{
	Super::InitAttributeSet();

	const float DashDamageRange = MonsterAttributeSet->GetMonsterData3();
	DashDamageRangeBox->SetBoxExtent(FVector(GetCapsuleComponent()->GetScaledCapsuleRadius(), DashDamageRange, DashDamageRange));
}

void ALLL_ManOfStrength::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (bIsDashing)
	{
		if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OtherActor))
		{
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ManOfStrengthDataAsset->DashDamageEffect, Level, EffectContextHandle);
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

void ALLL_ManOfStrength::Charge()
{
	AAIController* AIController = CastChecked<AAIController>(GetController());
	switch (static_cast<EBossMonsterPattern>(AIController->GetBlackboardComponent()->GetValueAsEnum(BBKEY_PATTERN)))
	{
	case EBossMonsterPattern::ManOfStrength_Dash:
		ChargeTimer = MonsterAttributeSet->GetChargeTimer();
		break;
		
	case EBossMonsterPattern::ManOfStrength_Shockwave:
		ChargeTimer = MonsterAttributeSet->GetMonsterData4();
		break;
		
	case EBossMonsterPattern::ManOfStrength_SnapOtherMonster:
		ChargeTimer = MonsterAttributeSet->GetMonsterData14();
		break;
		
	case EBossMonsterPattern::ManOfStrength_ApneaInAttack:
		ChargeTimer = MonsterAttributeSet->GetMonsterData9();
		break;
	}
	
	Super::Charge();
}

void ALLL_ManOfStrength::Dash()
{
	DashDistance = MonsterAttributeSet->GetMonsterData1();
	DashSpeed = MonsterAttributeSet->GetMonsterData2();
	
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_DASH)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 대쉬 펀치"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_ManOfStrength::Shockwave()
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MAN_OF_STRENGTH_SHOCKWAVE)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 충격파 생성"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_ManOfStrength::AttackInApnea()
{
	DashDistance = MonsterAttributeSet->GetMonsterData5();
	DashSpeed = MonsterAttributeSet->GetMonsterData6();
	
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MAN_OF_STRENGTH_ATTACK_IN_APNEA)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 무호흡 연타"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_ManOfStrength::SnapOtherMonster() const
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MAN_OF_STRENGTH_SNAP_OTHER_MONSTER)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 집어던지기 (잡기)"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_ManOfStrength::ThrowOtherMonster() const
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MAN_OF_STRENGTH_THROW_OTHER_MONSTER)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 집어던지기 (던지기)"), *GetName()));
			}
		}
#endif
	}
}
