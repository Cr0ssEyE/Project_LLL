// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Ability/LLL_RangeFeatherDetector.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Attribute/Object/Ability/LLL_RangeFeatherDetectorAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_RangeFeatherDetector::ALLL_RangeFeatherDetector()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RangeFeatherDetectorDataAsset>(PATH_RANGE_FEATHER_DETECTOR_DATA, EAssertionLevel::Check);

	RangeFeatherDetectorAttributeSet = CreateDefaultSubobject<ULLL_RangeFeatherDetectorAttributeSet>(TEXT("RangeFeatherDetectorAttributeSet"));
}

void ALLL_RangeFeatherDetector::BeginPlay()
{
	Super::BeginPlay();

	RangeFeatherDetectorDataAsset = Cast<ULLL_RangeFeatherDetectorDataAsset>(AbilityObjectDataAsset);
	AbilityObjectAttributeSet = RangeFeatherDetectorAttributeSet;

	CastChecked<ALLL_PlayerBase>(GetOwner())->SetFeatherSpawnStartTime(AbilityObjectAttributeSet->GetDestroyTimer() + 0.01f);

	OverlapCollisionSphere->SetSphereRadius(AbilityData->AbilityValue1 * 100.0f);
}

void ALLL_RangeFeatherDetector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 이펙트가 나오기 전까지 디버그 유지
	//DrawDebugSphere(GetWorld(), GetActorLocation(), OverlapCollisionSphere->GetScaledSphereRadius(), 16, FColor::Green, false, AbilityObjectAttributeSet->GetDestroyTimer());
}

void ALLL_RangeFeatherDetector::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	CastChecked<ALLL_PlayerBase>(GetOwner())->AddRangeFeatherTargets(OtherActor);
}

void ALLL_RangeFeatherDetector::Destroyed()
{
	Super::Destroyed();

	CastChecked<ALLL_PlayerBase>(GetOwner())->SetFeatherSpawnStartTime(0.01f);
}
