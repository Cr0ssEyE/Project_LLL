// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_StaffBasicMagic.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_StaffBasicMagicDataAsset.h"
#include "GAS/Attribute/Object/Thrown/LLL_StaffBasicMagicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_StaffBasicMagic::ALLL_StaffBasicMagic()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_StaffBasicMagicDataAsset>(PATH_STAFF_BASIC_MAGIC_DATA, EAssertionLevel::Check);

	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_StaffBasicMagicAttributeSet>(TEXT("StaffBasicMagicAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_MONSTER_ATTACK);
	SetRootComponent(HitCollisionBox);
}

void ALLL_StaffBasicMagic::BeginPlay()
{
	Super::BeginPlay();

	StaffBasicMagicDataAsset = Cast<ULLL_StaffBasicMagicDataAsset>(ThrownObjectDataAsset);

	HitCollisionBox->SetBoxExtent(StaffBasicMagicDataAsset->HitCollisionSize);
}

void ALLL_StaffBasicMagic::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ALLL_StaffBasicMagic::Deactivate()
{
	Super::Deactivate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
