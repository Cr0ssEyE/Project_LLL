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
	
	CorrectionReachStateDistance = WireObjectDataAsset->CorrectionReachStateDistance;
	// 이후 BeginPlay에서 InitEffect를 통해 실제 사용하는 값으로 초기화 해준다. 해당 매직넘버는 비정상적인 동작 방지용
	HandCollision->SetSphereRadius(100.f);
	RootComponent = HandCollision;
	HandMesh->SetupAttachment(RootComponent);

	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandMesh->SetHiddenInGame(true);
	
	bIsGrabbed = false;
}

void ALLL_PlayerWireHand::SetNormalState()
{
	bIsGrabbed = false;
	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandMesh->SetHiddenInGame(true);
	
	ProjectileMovement->Velocity = FVector::Zero();
	ProjectileMovement->Deactivate();
}

void ALLL_PlayerWireHand::SetThrowState(const FVector Location)
{
	SetActorLocation(GetOwner()->GetActorLocation());
	TargetLocation = Location;
	
	HandCollision->SetCollisionObjectType(ECC_ENEMY_ONLY);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::CheckReached);
}

void ALLL_PlayerWireHand::SetReleaseState(const FVector Location)
{
	TargetLocation = Location;
	
	HandCollision->SetCollisionObjectType(ECC_PLAYER_ONLY);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	
	FVector WorldLocation = GetActorLocation();
	K2_DetachFromActor();
	SetActorLocation(WorldLocation);
	
	ProjectileMovement->Activate();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
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
		SetNormalState();
		ReleaseCompleteDelegate.Broadcast();
		// FGameplayTagContainer ReleaseCompletedTag(TAG_GAS_PLAYER_WIRE_RETURN);
		// if(PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(ReleaseCompletedTag))
		// {
		// 	SetNormalState();
		// 	ReleaseCompleteDelegate.Broadcast();
		// }
	}
}

void ALLL_PlayerWireHand::CheckReached()
{
	if(bIsGrabbed)
	{
		return;
	}
	
	float LocationDistance = FVector::Distance(GetActorLocation(), TargetLocation);
	if(LocationDistance <= CorrectionReachStateDistance)
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
	if(HandCollision->GetCollisionObjectType() != ECC_PLAYER_ONLY)
	{
		return;
	}
	
	const FVector ToOwnerDirection = (GetOwner()->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	SetActorRotation(ToOwnerDirection.Rotation());
	ProjectileMovement->Velocity = ToOwnerDirection * WireHandAttributeSet->GetReleaseSpeed();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerWireHand::RetargetReleaseVelocity);
}

