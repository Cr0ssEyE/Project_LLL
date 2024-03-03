// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
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
	BaseObjectDataAsset = Cast<ULLL_BaseObjectDataAsset>(FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerWireObjectDataAsset>(PATH_PLAYER_WIRE_DATA, EAssertionLevel::Check));
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	WireHandAttributeSet = CreateDefaultSubobject<ULLL_PlayerWireHandAttributeSet>(TEXT("WireHandAttributeSet"));

	RootComponent = HandCollision;
	HandMesh->SetupAttachment(RootComponent);

	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALLL_PlayerWireHand::SetThrowState()
{
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
}

void ALLL_PlayerWireHand::SetReleaseState()
{
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->Activate();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::CheckReached);
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
	if(IsValid(Monster))
	{
		// PGA_WireHandGrab
		FGameplayTagContainer GrabTag;
		ASC->TryActivateAbilitiesByTag(GrabTag);
	}
}

void ALLL_PlayerWireHand::CheckReached()
{
	
}

void ALLL_PlayerWireHand::RetargetReleaseVelocity()
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
}

