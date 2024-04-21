// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/ChaseSystem/LLL_PGA_ChaseHandRelease.h"

#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/ThrownObject/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"

ULLL_PGA_ChaseHandRelease::ULLL_PGA_ChaseHandRelease()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_ChaseHandRelease::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->GetHandMesh()->SetAnimation(ReleaseAnim);

	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandRelease::OnCompleteCallBack);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandRelease::ReleaseToOwnerLocation);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 회수 시작")));
		}
	}
#endif
}

void ULLL_PGA_ChaseHandRelease::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ChaseHandRelease::OnCompleteCallBack);
	PlayerChaseHand->SetHiddenState();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 회수 종료")));
		}
	}
#endif
}

bool ULLL_PGA_ChaseHandRelease::CheckOwnerAlreadyOverlapped()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	float SphereRadius = PlayerChaseHand->GetCollisionComponent()->GetScaledSphereRadius();

	FHitResult Result;
	FCollisionQueryParams Params;

	GetWorld()->SweepSingleByChannel(
		Result,
		PlayerChaseHand->GetActorLocation(),
		PlayerChaseHand->GetActorLocation(),
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

void ULLL_PGA_ChaseHandRelease::ReleaseToOwnerLocation()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);

	const FVector WorldLocation = PlayerChaseHand->GetActorLocation();
	PlayerChaseHand->K2_DetachFromActor();
	PlayerChaseHand->SetActorLocation(WorldLocation);
	PlayerChaseHand->GetHandMesh()->SetAnimation(ReleaseAnim);
	
	USphereComponent* HandCollision = PlayerChaseHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerChaseHand->GetHandMesh();
	
	HandCollision->SetCollisionObjectType(ECC_PLAYER_CHECK);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	
	UProjectileMovementComponent* ChaseHandProjectile = PlayerChaseHand->GetProjectileMovementComponent();
	ChaseHandProjectile->Activate();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandRelease::RetargetReleaseVelocity);
}

void ULLL_PGA_ChaseHandRelease::RetargetReleaseVelocity()
{
	if(CheckOwnerAlreadyOverlapped())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());
	
	const FVector ToOwnerDirection = (PlayerCharacter->GetActorLocation() - PlayerChaseHand->GetActorLocation()).GetSafeNormal();
	PlayerChaseHand->SetActorRotation(ToOwnerDirection.Rotation());

	UProjectileMovementComponent* ChaseHandProjectile = PlayerChaseHand->GetProjectileMovementComponent();
	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = CastChecked<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));
	
	ChaseHandProjectile->Velocity = ToOwnerDirection * ChaseHandAttributeSet->GetReleaseSpeed();
	if(PlayerChaseHand->GetCollisionComponent()->GetCollisionObjectType() == ECC_PLAYER_CHECK)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandRelease::RetargetReleaseVelocity);
	}
}

