﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "DataAsset/LLL_PlayerWireObjectDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"
#include "Util/LLLConstructorHelper.h"


// Sets default values
ALLL_PlayerWireHand::ALLL_PlayerWireHand()
{
	ULLL_PlayerWireObjectDataAsset* WireObjectDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerWireObjectDataAsset>(PATH_PLAYER_WIRE_DATA, EAssertionLevel::Check);
	BaseObjectDataAsset = Cast<ULLL_BaseObjectDataAsset>(WireObjectDataAsset);
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	WireHandAttributeSet = CreateDefaultSubobject<ULLL_PlayerWireHandAttributeSet>(TEXT("WireHandAttributeSet"));

	HandMesh->SetSkeletalMesh(WireObjectDataAsset->SkeletalMesh);
	
	CorrectionReachStateDistance = WireObjectDataAsset->CorrectionReachStateDistance;
	// 이후 BeginPlay에서 InitEffect를 통해 실제 사용하는 값으로 초기화 해준다. 해당 매직넘버는 비정상적인 동작 방지용
	HandCollision->SetSphereRadius(100.f);
	RootComponent = HandCollision;
	HandMesh->SetupAttachment(RootComponent);

	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsGrabbed = false;
}

void ALLL_PlayerWireHand::SetNormalState()
{
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->Velocity = FVector::Zero();
	ProjectileMovement->Deactivate();
}

void ALLL_PlayerWireHand::SetThrowState(const FVector Location)
{
	TargetLocation = Location;
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandCollision->SetCollisionObjectType(ECC_ENEMY_ONLY);
	ProjectileMovement->Activate();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::CheckReached);
}

void ALLL_PlayerWireHand::SetReleaseState(const FVector Location)
{
	TargetLocation = Location;
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandCollision->SetCollisionObjectType(ECC_PLAYER_ONLY);
	ProjectileMovement->Activate();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
}

void ALLL_PlayerWireHand::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->AddSpawnedAttribute(WireHandAttributeSet);
}

// Called when the game starts or when spawned
void ALLL_PlayerWireHand::BeginPlay()
{
	Super::BeginPlay();
	if(ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetParentActor()))
	{
		SetOwner(PlayerCharacter);
	}
}

void ALLL_PlayerWireHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if(IsValid(Monster) && HandCollision->GetCollisionObjectType() == ECC_ENEMY_ONLY)
	{
		// PGA_WireHandGrab
		FGameplayTagContainer GrabTag;
		if(ASC->TryActivateAbilitiesByTag(GrabTag))
		{
			bIsGrabbed = true;
			return;
		}
	}

	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor);
	if(IsValid(PlayerCharacter) && HandCollision->GetCollisionObjectType() == ECC_PLAYER_ONLY)
	{
		// PGA
		FGameplayTagContainer ReleaseCompletedTag;
		if(ASC->TryActivateAbilitiesByTag(ReleaseCompletedTag))
		{
			SetNormalState();
			ReleaseCompleteDelegate.Broadcast();
		}
	}
}

void ALLL_PlayerWireHand::CheckReached()
{
	float LocationDistance = FVector::Distance(GetActorLocation(), TargetLocation);
	if(LocationDistance <= CorrectionReachStateDistance || bIsGrabbed)
	{
		SetNormalState();
		ThrowCompleteDelegate.Broadcast();
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::CheckReached);
}

void ALLL_PlayerWireHand::RetargetReleaseVelocity()
{
	// TODO: 매 틱마다 플레이어 위치 기준으로 방향 조정하기
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
}

