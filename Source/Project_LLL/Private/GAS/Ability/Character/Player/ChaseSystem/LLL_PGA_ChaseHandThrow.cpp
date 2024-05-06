// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseHandThrow.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHandAttributeSet.h"
#include "Util/LLL_ExecuteCueHelper.h"

ULLL_PGA_ChaseHandThrow::ULLL_PGA_ChaseHandThrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = FVector::Zero();
}

void ULLL_PGA_ChaseHandThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());

	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	PlayerChaseHand->OnGrabbedDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	PlayerCharacter->PlayerRotateToMouseCursor();
	
	ThrowToCursorLocation();
}

void ULLL_PGA_ChaseHandThrow::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	PlayerChaseHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	PlayerChaseHand->OnGrabbedDelegate.RemoveDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	
	// Cancel되지 않고 종료한 경우 = 마우스 커서 위치에 도달해 회수 상태로 전환한 경우
	if (!bWasCancelled)
	{
		// 단, 도달한 위치 주변에 몬스터가 있다면 그랩으로 전환
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 종료. 주변 대상 그랩 시도")));
			}
		}
#endif
		if (!GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASE_GRAB)))
		{
			GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASE_RELEASE));
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
			{
				if(ProtoGameInstance->CheckPlayerChaseActionDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 종료 후 그랩 실패. 회수 시작")));
				}
			}
#endif
		}
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ChaseHandThrow::ThrowToCursorLocation()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());
	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = CastChecked<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));
	
	TargetLocation = PlayerCharacter->GetMouseLocation();
	const float TargetDistance = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	// 마우스 위치가 투척 최소거리 보다 가까운 거리일 경우 보정
	if (TargetDistance < ChaseHandAttributeSet->GetMinimumThrowDistance())
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 최소거리 보정. 보정 전 좌표 : %f, %f, %f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z));
			}
		}
#endif
		TargetLocation *= ChaseHandAttributeSet->GetMinimumThrowDistance() / TargetDistance;
	}
	
	TargetLocation.Z = PlayerCharacter->GetActorLocation().Z;
	
	const FVector ThrowDirection = PlayerCharacter->GetActorForwardVector();

	PlayerChaseHand->SetActorLocationAndRotation(PlayerCharacter->GetActorLocation(), ThrowDirection.Rotation());
	PlayerChaseHand->SetActorRotation(ThrowDirection.Rotation());
	
	USphereComponent* HandCollision = PlayerChaseHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerChaseHand->GetHandMesh();
	
	HandCollision->SetCollisionProfileName(CP_PLAYER_CHASE_HAND);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HandMesh->SetHiddenInGame(false);
	HandMesh->SetAnimation(ThrowAnim);
	
	UProjectileMovementComponent* HandProjectile = PlayerChaseHand->GetProjectileMovementComponent();
	HandProjectile->Activate();
	HandProjectile->Velocity = ThrowDirection * ChaseHandAttributeSet->GetThrowSpeed();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandThrow::CheckReached);

	FLLL_ExecuteCueHelper::ExecuteCue(PlayerCharacter, WireHandThrowCueTag);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 시작. 투사체 속도 : %f"), ChaseHandAttributeSet->GetThrowSpeed()));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 시작. 목표 좌표 : %f, %f, %f"), TargetLocation.X, TargetLocation.Y, TargetLocation.Z));
		}
	}
#endif
}

void ULLL_PGA_ChaseHandThrow::CheckReached()
{
	if (GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASE_GRAB)))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		return;
	}
	
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());
	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = CastChecked<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));

	const float LocationDistance = FVector::Distance(PlayerChaseHand->GetActorLocation(), TargetLocation);
	const float OwnerDistance = FVector::DistXY(PlayerChaseHand->GetActorLocation(), PlayerCharacter->GetActorLocation());
	
	if (LocationDistance <= ChaseHandAttributeSet->GetCorrectionReachStateDistance())
	{
		PlayerChaseHand->SetActorLocation(TargetLocation);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else if (OwnerDistance >= ChaseHandAttributeSet->GetMaximumThrowDistance())
	{
		GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASE_RELEASE));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandThrow::CheckReached);
	}
}
