// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownMagic.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_ThrownMagicDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownMagicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownMagic::ALLL_ThrownMagic()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownMagicDataAsset>(PATH_THROWN_MAGIC_DATA, EAssertionLevel::Check);

	ThrownMagicAttributeSet = CreateDefaultSubobject<ULLL_ThrownMagicAttributeSet>(TEXT("StaffBasicMagicAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_MONSTER_ATTACK);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownMagic::BeginPlay()
{
	Super::BeginPlay();

	ThrownMagicDataAsset = Cast<ULLL_ThrownMagicDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownMagicAttributeSet;

	HitCollisionBox->SetBoxExtent(ThrownMagicDataAsset->HitCollisionSize);
}

void ALLL_ThrownMagic::Tick(float DeltaSeconds)
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

void ALLL_ThrownMagic::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_ThrownMagic::Deactivate()
{
	Super::Deactivate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALLL_ThrownMagic::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	Super::Throw(NewOwner, NewTarget, InSpeed, Straight, InKnockBackPower);

	if (const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(GetOwner()))
	{
		const UAbilitySystemComponent* MonsterASC = Monster->GetAbilitySystemComponent();
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(MonsterASC->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		OffencePower = MonsterAttributeSet->GetOffencePower();
	}
}
