// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandRelease.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandRelease::ULLL_PGA_WireHandRelease()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_WireHandRelease::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->GetHandMesh()->SetAnimation(ReleaseAnim);

	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandRelease::ReleaseToOwnerLocation);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerWireActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 회수 시작")));
		}
	}
#endif
}

void ULLL_PGA_WireHandRelease::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack);
	PlayerWireHand->SetHiddenState();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerWireActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 회수 종료")));
		}
	}
#endif
}

bool ULLL_PGA_WireHandRelease::CheckOwnerAlreadyOverlapped()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	float SphereRadius = PlayerWireHand->GetCollisionComponent()->GetScaledSphereRadius();

	FHitResult Result;
	FCollisionQueryParams Params;

	GetWorld()->SweepSingleByChannel(
		Result,
		PlayerWireHand->GetActorLocation(),
		PlayerWireHand->GetActorLocation(),
		FQuat::Identity,
		ECC_PLAYER_CHECK,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if(Result.GetActor())
	{
		return true;
	}
	return false;
}

void ULLL_PGA_WireHandRelease::ReleaseToOwnerLocation()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());

	const FVector WorldLocation = PlayerWireHand->GetActorLocation();
	PlayerWireHand->K2_DetachFromActor();
	PlayerWireHand->SetActorLocation(WorldLocation);
	PlayerWireHand->GetHandMesh()->SetAnimation(ReleaseAnim);
	
	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	WireHandCollision->SetCollisionObjectType(ECC_PLAYER_CHECK);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	
	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	WireHandProjectile->Activate();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandRelease::RetargetReleaseVelocity);
}

void ULLL_PGA_WireHandRelease::RetargetReleaseVelocity()
{
	if(CheckOwnerAlreadyOverlapped())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	
	const FVector ToOwnerDirection = (PlayerCharacter->GetActorLocation() - PlayerWireHand->GetActorLocation()).GetSafeNormal();
	PlayerWireHand->SetActorRotation(ToOwnerDirection.Rotation());

	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	const ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = CastChecked<ULLL_PlayerWireHandAttributeSet>(PlayerWireHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerWireHandAttributeSet::StaticClass()));
	
	WireHandProjectile->Velocity = ToOwnerDirection * WireHandAttributeSet->GetReleaseSpeed();
	if(PlayerWireHand->GetCollisionComponent()->GetCollisionObjectType() == ECC_PLAYER_CHECK)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandRelease::RetargetReleaseVelocity);
	}
}

void ULLL_PGA_WireHandRelease::OnReleaseCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
