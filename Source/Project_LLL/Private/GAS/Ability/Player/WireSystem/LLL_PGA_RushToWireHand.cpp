// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_RushToWireHand.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerWireHand/LLL_PlayerWireHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Ability/Player/WireSystem/LLL_PGA_ControlWireHand.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"

ULLL_PGA_RushToWireHand::ULLL_PGA_RushToWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	TargetLocation = Direction = FVector::Zero();
	AbilityEndDistance = 200.f;
	RushSpeed = 0.f;
}

void ULLL_PGA_RushToWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
	PlayerCharacter->SetActorRotation((PlayerWireHand->GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal().Rotation());
	PlayerCharacter->GetCharacterMovement()->Velocity = FVector::Zero();

	PlayerWireHand->GetCollisionComponent()->SetCollisionObjectType(ECC_PLAYER_CHECK);
	PlayerWireHand->GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	TargetLocation = PlayerWireHand->GetActorLocation();
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	RushSpeed = PlayerAttributeSet->GetRushSpeed();

	UAbilityTask_WaitGameplayTagAdded* WaitTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_GAS_COLLIDE_WALL);
	WaitTask->Added.AddDynamic(this, &ULLL_PGA_RushToWireHand::OnCollidedCallBack);
	WaitTask->ReadyForActivation();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand);

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

void ULLL_PGA_RushToWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	PlayerCharacter->GetCharacterMovement()->Velocity = PlayerCharacter->GetCharacterMovement()->Velocity.GetSafeNormal() * PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	
	PlayerWireHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_RELEASE));
	
	PlayerCharacter->GetAbilitySystemComponent()->CancelAbilities( new FGameplayTagContainer(TAG_GAS_PLAYER_WIRE_THROW));

	if (!bWasCancelled)
	{
		PlayerCharacter->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_PLAYER_WIRE_ATTACK));
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

void ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand()
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	float Distance2D = FVector::DistXY(TargetLocation, PlayerCharacter->GetActorLocation());
	
	if(RushSpeed <= 0.f || Distance2D < AbilityEndDistance)
	{
		PlayerWireHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_WIRE_RELEASE));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	Direction = (TargetLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	PlayerCharacter->GetCharacterMovement()->Velocity += Direction * RushSpeed;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_RushToWireHand::OwnerLaunchToWireHand);
}

void ULLL_PGA_RushToWireHand::OnCollidedCallBack()
{
	RushSpeed = 0.f;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
