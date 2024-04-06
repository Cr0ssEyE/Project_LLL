// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandThrow.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandThrow::ULLL_PGA_WireHandThrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = FVector::Zero();
}

void ULLL_PGA_WireHandThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());

	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_WireHandThrow::OnInterruptedCallBack);
	PlayerWireHand->OnGrabbedDelegate.AddDynamic(this, &ULLL_PGA_WireHandThrow::OnInterruptedCallBack);
	PlayerCharacter->PlayerRotateToMouseCursor();
	
	ThrowToCursorLocation();
}

void ULLL_PGA_WireHandThrow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandThrow::OnInterruptedCallBack);
	PlayerWireHand->OnGrabbedDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandThrow::OnInterruptedCallBack);
	
	// Cancel되지 않고 종료한 경우 = 마우스 커서 위치에 도달해 회수 상태로 전환한 경우
	if (!bWasCancelled)
	{
		// 단, 도달한 위치 주변에 몬스터가 있다면 그랩으로 전환
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerWireActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 종료. 주변 대상 그랩 시도")));
			}
		}
#endif
		if (!GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_GRAB)))
		{
			GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_RELEASE));
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if(ProtoGameInstance->CheckPlayerWireActionDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 종료 후 그랩 실패. 회수 시작")));
				}
			}
#endif
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_WireHandThrow::ThrowToCursorLocation()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	const ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = CastChecked<ULLL_PlayerWireHandAttributeSet>(PlayerWireHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerWireHandAttributeSet::StaticClass()));
	
	TargetLocation = PlayerCharacter->GetMouseLocation();
	float TargetDistance = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	// 마우스 위치가 투척 최소거리 보다 가까운 거리일 경우 보정
	if (TargetDistance < WireHandAttributeSet->GetMinimumThrowDistance())
	{
		TargetLocation *= WireHandAttributeSet->GetMinimumThrowDistance() / TargetDistance;
	}
	TargetLocation.Z = PlayerCharacter->GetActorLocation().Z;
	
	const FVector ThrowDirection = PlayerCharacter->GetActorForwardVector();

	PlayerWireHand->SetActorLocationAndRotation(PlayerCharacter->GetActorLocation(), ThrowDirection.Rotation());
	PlayerWireHand->SetActorRotation(ThrowDirection.Rotation());
	
	USphereComponent* WireHandCollision = PlayerWireHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerWireHand->GetHandMesh();
	
	WireHandCollision->SetCollisionObjectType(ECC_ENEMY_HIT);
	WireHandCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandMesh->SetHiddenInGame(false);
	HandMesh->SetAnimation(ThrowAnim);
	
	UProjectileMovementComponent* WireHandProjectile = PlayerWireHand->GetProjectileComponent();
	WireHandProjectile->Activate();
	WireHandProjectile->Velocity = ThrowDirection * WireHandAttributeSet->GetThrowSpeed();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandThrow::CheckReached);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerWireActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 시작. 목표 좌표 : %f, %f, %f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z));
		}
	}
#endif
}

void ULLL_PGA_WireHandThrow::CheckReached()
{
	if (GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_GRAB)))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	const ULLL_PlayerWireHandAttributeSet* WireHandAttributeSet = CastChecked<ULLL_PlayerWireHandAttributeSet>(PlayerWireHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerWireHandAttributeSet::StaticClass()));
	
	float LocationDistance = FVector::Distance(PlayerWireHand->GetActorLocation(), TargetLocation);
	float OwnerDistance = FVector::DistXY(PlayerWireHand->GetActorLocation(), PlayerCharacter->GetActorLocation());
	
	if (LocationDistance <= WireHandAttributeSet->GetCorrectionReachStateDistance())
	{
		PlayerWireHand->SetActorLocation(TargetLocation);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else if (OwnerDistance >= WireHandAttributeSet->GetMaximumThrowDistance())
	{
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_RELEASE));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandThrow::CheckReached);
	}
}
