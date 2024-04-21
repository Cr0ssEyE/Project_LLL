// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"

#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "DataAsset/LLL_PlayerChaseHandDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/ThrownObject/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerChaseHand::ALLL_PlayerChaseHand()
{
	ThrownObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerChaseHandDataAsset>(PATH_PLAYER_CHASE_HAND_DATA, EAssertionLevel::Check);
	BaseObjectDataAsset = Cast<ULLL_BaseObjectDataAsset>(ThrownObjectDataAsset);
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_PlayerChaseHandAttributeSet>(TEXT("ChaseHandAttributeSet"));

	HandMesh->SetSkeletalMesh(ThrownObjectDataAsset->SkeletalMesh);
	HandMesh->SetRelativeScale3D(ThrownObjectDataAsset->MeshScale);
	HandMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	// 이후 BeginPlay에서 InitEffect를 통해 실제 사용하는 값으로 초기화 해준다. 해당 매직넘버는 비정상적인 동작 방지용
	HandCollision->SetSphereRadius(100.f);
	RootComponent = HandCollision;
	HandMesh->SetupAttachment(RootComponent);

	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandMesh->SetHiddenInGame(true);
	
	bIsGrabbed = false;
}

void ALLL_PlayerChaseHand::SetHiddenState()
{
	bIsGrabbed = false;
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandMesh->SetHiddenInGame(true);
	
	ProjectileMovementComponent->Velocity = FVector::Zero();
	ProjectileMovementComponent->Deactivate();

	SetActorLocation(GetOwner()->GetActorLocation());
}

void ALLL_PlayerChaseHand::BeginPlay()
{
	Super::BeginPlay();
}

void ALLL_PlayerChaseHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if(IsValid(Monster) && HandCollision->GetCollisionObjectType() == ECC_ENEMY_HIT)
	{
		if(bIsGrabbed)
		{
			return;
		}
		
		// PGA_WireHandGrab
		const FGameplayTagContainer GrabTag(TAG_GAS_WIRE_GRAB);
		if(ASC->TryActivateAbilitiesByTag(GrabTag))
		{
			GrabbedActor = OtherActor;
			bIsGrabbed = true;
			OnGrabbedDelegate.Broadcast();
			return;
		}
	}

	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor);
	if(IsValid(PlayerCharacter) && HandCollision->GetCollisionObjectType() == ECC_PLAYER_CHECK)
	{
		SetHiddenState();
		ReleaseCompleteDelegate.Broadcast();
	}
}