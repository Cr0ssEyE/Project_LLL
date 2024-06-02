﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_Dash.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_MathHelper.h"

ULLL_PGA_Dash::ULLL_PGA_Dash()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentDashCount = 0;
}

void ULLL_PGA_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 발동")));
		}
	}
#endif
	
	// ASC에 등록된 어트리뷰트 가져오고 GA에서 필요한 어트리뷰트 저장하기
	const ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet))
	{
		DashSpeed = PlayerAttributeSet->GetDashSpeed();
		DashCorrectionDistance = PlayerAttributeSet->GetDashCorrectionDistance();
		MaxDashCount = PlayerAttributeSet->GetMaxDashCount();
	}
	bIsInputPressed = true;
	MontageStop();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_Dash::DashActionEvent);
}

void ULLL_PGA_Dash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckPlayerDashDebug())
		{
			if(bWasCancelled)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("대쉬 어빌리티 중단됨")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 어빌리티 종료")));
			}
		}
	}
#endif

	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	CurrentDashCount = 0;

	if (IsValid(DashTask) && DashTask->IsActive())
	{
		DashTask->EndTask();
	}

	PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	const TArray<FGameplayAbilitySpec> ActiveAbilitySpec = GetAbilitySystemComponentFromActorInfo_Checked()->GetActivatableAbilities();
	for (auto AbilitySpec : ActiveAbilitySpec)
	{
		if (AbilitySpec.Ability->AbilityTags.HasTag(TAG_GAS_PLAYER_CHASE_RUSH))
		{
			PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER_EVADE);
			break;
		}
	}
	
	ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(PlayerCharacter->GetCharacterAnimInstance());
	PlayerAnimInstance->SetDash(false);
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_Dash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
	{
		bIsInputPressed = true;
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(DebugGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 추가 입력 인식")));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::DashActionEvent()
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	if (IsValid(PlayerCharacter) && bIsInputPressed && CurrentDashCount < MaxDashCount)
	{
		bIsInputPressed = false;
		CurrentDashCount++;
		
		FVector DashDirection;
		if (PlayerCharacter->GetMoveInputPressed())
		{
			DashDirection = PlayerCharacter->GetMoveInputDirection().GetSafeNormal2D();
		}
		else
		{
			DashDirection = PlayerCharacter->GetActorForwardVector().GetSafeNormal2D();
		}

		DashDistance = PlayerCharacterAttributeSet->GetDashDistance();
		const FVector DashLocation = FLLL_MathHelper::CalculatePlayerLaunchableLocation(GetWorld(), PlayerCharacter, DashDistance, DashCorrectionDistance, DashDirection);
		PlayerCharacter->GetMovementComponent()->Velocity = FVector::Zero();
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER_EVADE);
		
		if (IsValid(DashTask) && DashTask->IsActive())
		{
			DashTask->EndTask();
		}
		DashTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Dash"), DashLocation, DashDistance / DashSpeed, nullptr, nullptr);
		DashTask->OnTargetLocationReached.AddDynamic(this, &ULLL_PGA_Dash::LocationReachedEvent);
		DashTask->ReadyForActivation();

		if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
		{
			WaitTagTask->EndTask();
		}
		WaitTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_STATE_INPUT_CHECK_DASH, nullptr, true);
		WaitTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_Dash::CheckInputPressed);
		WaitTagTask->ReadyForActivation();
		
		// 애님 몽타주 처음부터 다시 실행하거나 특정 시간부터 실행 시키도록 하는게 상당히 귀찮아서 땜빵 처리
		PlayerCharacter->StopAnimMontage(DashAnimMontage);
		PlayerCharacter->PlayAnimMontage(DashAnimMontage);

		ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(PlayerCharacter->GetCharacterAnimInstance());
		PlayerAnimInstance->SetDash(true);

		const FGameplayEventData PayloadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_DASH_START, PayloadData);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(DebugGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 액션 발동. 현재 횟수: %d, 최대 횟수: %d"), CurrentDashCount, MaxDashCount));
			}
		}
#endif
	}
}

void ULLL_PGA_Dash::LocationReachedEvent()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("대쉬 이동 완료")));
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
	}
	const FGameplayEventData PayloadData;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_PLAYER_DASH_END, PayloadData);
}

void ULLL_PGA_Dash::CheckInputPressed(FGameplayEventData EventData)
{
 	if (!bIsInputPressed || CurrentDashCount >= MaxDashCount)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_Dash::DashActionEvent);
}
