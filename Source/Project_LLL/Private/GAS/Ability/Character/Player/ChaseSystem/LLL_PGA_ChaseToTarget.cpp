// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseToTarget.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_PlayerChaseHandAttributeSet.h"

ULLL_PGA_ChaseToTarget::ULLL_PGA_ChaseToTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = Direction = FVector::Zero();
	AbilityEndDistance = 200.f;
	RushSpeed = 0.f;
}

void ULLL_PGA_ChaseToTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();
	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = Cast<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));
	
	// PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER_EVADE);
	PlayerCharacter->SetActorRotation((PlayerChaseHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal().Rotation());
	PlayerCharacter->GetCharacterMovement()->Velocity = FVector::Zero();

	PlayerChaseHand->GetCollisionComponent()->SetCollisionObjectType(ECC_PLAYER_CHECK);
	PlayerChaseHand->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetLocation = PlayerChaseHand->GetActorLocation();
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	RushSpeed = ChaseHandAttributeSet->GetChaseSpeed();

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), AbilityActionMontage, 1.0f, SECTION_FLY);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_ChaseToTarget::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_ChaseToTarget::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* WaitTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_COLLIDE_WALL, nullptr, true);
	WaitTask->EventReceived.AddDynamic(this, &ULLL_PGA_ChaseToTarget::OnCollideCallBack);
	WaitTask->ReadyForActivation();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseToTarget::OwnerLaunchToChaseHand);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 돌진 어빌리티 발동")));
		}
	}
#endif
}

void ULLL_PGA_ChaseToTarget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	RushSpeed = 0.f;
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();
	
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	PlayerCharacter->GetCharacterMovement()->Velocity = PlayerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal() * PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	
	PlayerChaseHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_RELEASE));
	
	PlayerCharacter->GetAbilitySystemComponent()->CancelAbilities( new FGameplayTagContainer(TAG_GAS_PLAYER_CHASER_THROW));

	if (!bWasCancelled)
	{
		PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_PLAYER_CHASE_ATTACK));
	}
	
	TargetLocation = Direction = FVector::Zero();
	RushSpeed = 0.f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("와이어 돌진 어빌리티 종료")));
		}
	}
#endif
}

void ULLL_PGA_ChaseToTarget::OwnerLaunchToChaseHand()
{
	if (bIsAbilityEnding)
	{
		return;
	}
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();
	const float Distance2D = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	
	if(RushSpeed <= 0.f || Distance2D < AbilityEndDistance)
	{
		PlayerChaseHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_RELEASE));
		PlayerCharacter->GetCharacterAnimInstance()->Montage_JumpToSectionsEnd(SECTION_FLY, AbilityActionMontage);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	PlayerCharacter->GetCharacterMovement()->Velocity += Direction * RushSpeed;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_ChaseToTarget::OwnerLaunchToChaseHand);
}

void ULLL_PGA_ChaseToTarget::OnCollideCallBack(FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

