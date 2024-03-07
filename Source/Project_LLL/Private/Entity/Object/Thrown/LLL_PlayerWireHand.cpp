// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
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
	HandMesh->SetRelativeScale3D(WireObjectDataAsset->MeshScale);
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

void ALLL_PlayerWireHand::SetHiddenState()
{
	bIsGrabbed = false;
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandMesh->SetHiddenInGame(true);
	SetActorLocation(GetOwner()->GetActorLocation());
	
	ProjectileMovement->Velocity = FVector::Zero();
	ProjectileMovement->Deactivate();
}

void ALLL_PlayerWireHand::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ASC->AddSpawnedAttribute(WireHandAttributeSet);
}

void ALLL_PlayerWireHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if(IsValid(Monster) && HandCollision->GetCollisionObjectType() == ECC_ENEMY_ONLY)
	{
		if(bIsGrabbed)
		{
			return;
		}
		
		// PGA_WireHandGrab
		FGameplayTagContainer GrabTag(TAG_GAS_WIRE_GRAB);
		if(ASC->TryActivateAbilitiesByTag(GrabTag))
		{
			HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToActor(OtherActor, FAttachmentTransformRules::KeepRelativeTransform);
			SetActorLocation(OtherActor->GetActorLocation());
			ProjectileMovement->Velocity = FVector::Zero();
			ProjectileMovement->Deactivate();
			bIsGrabbed = true;
			OnGrabbedDelegate.Broadcast();
			return;
		}
	}

	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(OtherActor);
	if(IsValid(PlayerCharacter) && HandCollision->GetCollisionObjectType() == ECC_PLAYER_ONLY)
	{
		SetHiddenState();
		ReleaseCompleteDelegate.Broadcast();
	}
}