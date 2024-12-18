// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_RangeKnockBackDetector.h"

#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_RangeKnockBackDetectorDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Object/Ability/LLL_RangeKnockBackDetectorAttributeSet.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_RangeKnockBackDetector::ALLL_RangeKnockBackDetector()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RangeKnockBackDetectorDataAsset>(PATH_RANGE_KNOCK_BACK_DETECTOR_DATA, EAssertionLevel::Check);

	RangeKnockBackDetectorAttributeSet = CreateDefaultSubobject<ULLL_RangeKnockBackDetectorAttributeSet>(TEXT("RangeKnockBackDetectorAttributeSet"));
	
	OverlapCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Box Collision"));
	OverlapCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapCollisionBox->SetCollisionProfileName(CP_PLAYER_ABILITY_OBJECT);
	OverlapCollisionBox->SetupAttachment(RootComponent);
}

void ALLL_RangeKnockBackDetector::BeginPlay()
{
	Super::BeginPlay();

	RangeKnockBackDetectorDataAsset = Cast<ULLL_RangeKnockBackDetectorDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = RangeKnockBackDetectorAttributeSet;
	
	OverlapCollisionBox->SetBoxExtent(RangeKnockBackDetectorDataAsset->OverlapCollisionScale);
	// 이펙트가 나오기 전까지 디버그 유지
	OverlapCollisionBox->SetHiddenInGame(false);
}

void ALLL_RangeKnockBackDetector::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
	{
		KnockBackDirection = Player->GetActorForwardVector();
		
		KnockBackPower = AbilityData->AbilityValue1 * 100.0f;
		KnockBackPower = FLLL_AbilityDataHelper::CalculateKnockBackPower(KnockBackPower, Player);
		
		KnockBackTo(OtherActor);
	}
}
