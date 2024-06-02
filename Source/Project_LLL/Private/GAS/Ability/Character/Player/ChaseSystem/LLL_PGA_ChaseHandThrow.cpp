// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseHandThrow.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_PlayerChaseHandAttributeSet.h"
#include "Util/LLL_FModPlayHelper.h"

ULLL_PGA_ChaseHandThrow::ULLL_PGA_ChaseHandThrow()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	StartLocation = TargetLocation = FVector::Zero();
}

void ULLL_PGA_ChaseHandThrow::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);

	if (IsValid(PlayerChaseHand->GetAttachParentActor()))
	{
		PlayerChaseHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	PlayerChaseHand->OnGrabbedDelegate.AddDynamic(this, &ULLL_PGA_ChaseHandThrow::OnInterruptedCallBack);
	
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
		if(const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 종료. 주변 대상 그랩 시도")));
			}
		}
#endif
		if (!GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_GRAB)))
		{
			GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_RELEASE));
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if(const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
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
	
	StartLocation = PlayerCharacter->GetMesh()->GetSocketLocation(SOCKET_LEFT_WEAPON);
	TargetLocation = PlayerCharacter->GetLastCheckedMouseLocation();
	TargetLocation.Z = StartLocation.Z;
	const float TargetDistance =  FVector::Distance(TargetLocation, StartLocation);
	// 마우스 위치가 투척 최소거리 보다 가까운 거리일 경우 보정
	if (TargetDistance < ChaseHandAttributeSet->GetMinimumThrowDistance())
	{
		TargetLocation = StartLocation + (TargetLocation - StartLocation).GetSafeNormal2D() * ChaseHandAttributeSet->GetMinimumThrowDistance();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("와이어 투사체 최소거리 보정 동작.")));
			}
		}
#endif
		
	}
	
	TargetLocation.Z = StartLocation.Z;
	
	const FVector ThrowDirection = (TargetLocation - StartLocation).GetSafeNormal2D();

	PlayerChaseHand->SetActorLocationAndRotation(StartLocation, ThrowDirection.Rotation());
	
	USphereComponent* HandCollision = PlayerChaseHand->GetCollisionComponent();
	USkeletalMeshComponent* HandMesh = PlayerChaseHand->GetHandMesh();
	
	HandCollision->SetCollisionProfileName(CP_PLAYER_CHASE_HAND);
	HandMesh->SetHiddenInGame(false);
	HandMesh->SetAnimation(ThrowAnim);
	
	UProjectileMovementComponent* HandProjectile = PlayerChaseHand->GetProjectileMovementComponent();
	HandProjectile->SetUpdatedComponent(PlayerChaseHand->GetRootComponent());
	HandProjectile->Activate();
	HandProjectile->MaxSpeed = ChaseHandAttributeSet->GetThrowSpeed();
	HandProjectile->Velocity = ThrowDirection * ChaseHandAttributeSet->GetThrowSpeed();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandThrow::CheckReached);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 투사체 이동 거리 : %f"), FVector::Distance(TargetLocation, StartLocation)));
		}
	}
#endif
}

void ULLL_PGA_ChaseHandThrow::CheckReached()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = CastChecked<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));

	const float LocationDistance = FVector::Distance(PlayerChaseHand->GetActorLocation(), TargetLocation);
	const float MoveDistance =  FVector::Distance(PlayerChaseHand->GetActorLocation(), StartLocation);
	if (LocationDistance <= ChaseHandAttributeSet->GetCorrectionReachStateDistance())
	{
		PlayerChaseHand->SetActorLocation(TargetLocation);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else if (MoveDistance >= ChaseHandAttributeSet->GetMaximumThrowDistance())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseHandThrow::CheckReached);
	}
}
