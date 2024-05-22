﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackBase.h"

#include "FMODAudioComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTagBase.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_ExecuteCueHelper.h"

ULLL_PGA_AttackBase::ULLL_PGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboAction = 0;
	MaxAttackAction = 0;
	bIsCanPlayNextAction = false;
	bIsInputPressed = false;
}

void ULLL_PGA_AttackBase::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter == EFModParameter::PlayerAttackCountParameter)
		{
			PlayerAttackCountParameterName = FModParameterData.Name;
		}
	}
}

void ULLL_PGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("공격 어빌리티 발동")));
		}
	}
#endif
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet) && IsValid(AttackAnimMontage))
	{
		MaxAttackAction = PlayerAttributeSet->GetMaxAttackActionCount();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	PlayerCharacter->PlayerRotateToMouseCursor();
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackAnimMontage, 1.f, *FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
	MontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_AttackBase::OnCompleteCallBack);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_AttackBase::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();

	WaitTagTask = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this, TAG_GAS_PLAYER_STATE_WAIT_ATTACK_INPUT);
	WaitTagTask->Added.AddDynamic(this, &ULLL_PGA_AttackBase::WaitInputForNextAction);
	WaitTagTask->ReadyForActivation();

	PlayerCharacter->SetAttacking(true);
	ExecuteAttackCueWithDelay();
}

void ULLL_PGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug())
		{
			if(bWasCancelled)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("공격 어빌리티 중단됨")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("공격 어빌리티 종료")));
			}
		}
	}
#endif

	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurrentComboAction = 0;
		bIsCanPlayNextAction = false;
		GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
		WaitInputTimerHandle.Invalidate();
		PlayerCharacter->SetAttacking(false);
	}
	GetAbilitySystemComponentFromActorInfo_Checked()->CancelAbilities(new FGameplayTagContainer(TAG_GAS_ATTACK_HIT_CHECK));
	WaitTagTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_AttackBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if(bIsCanPlayNextAction)
	{
		SetNextAttackAction();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckPlayerAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("공격 추가 입력 인식")));
			}
		}
#endif
	}
}

void ULLL_PGA_AttackBase::WaitInputForNextAction()
{
	GetAbilitySystemComponentFromActorInfo_Checked()->RemoveLooseGameplayTag(TAG_GAS_PLAYER_STATE_WAIT_ATTACK_INPUT);
	bIsCanPlayNextAction = true;
}

void ULLL_PGA_AttackBase::SetNextAttackAction()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
	WaitInputTimerHandle.Invalidate();
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const UAnimInstance* PlayerAnimInstance = PlayerCharacter->GetCharacterAnimInstance();
	if(IsValid(PlayerCharacter) && bIsCanPlayNextAction && PlayerAnimInstance->Montage_IsPlaying(AttackAnimMontage))
	{
		FGameplayTagContainer OwnedTagsContainer;
		GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags(OwnedTagsContainer);
		if (OwnedTagsContainer.HasTag(TAG_GAS_PLAYER_STATE_CHASE_THREW))
		{
			CurrentComboAction++;
		}
		
		if(CurrentComboAction == MaxAttackAction)
		{
			CurrentComboAction = 0;
		}
		
		PlayerCharacter->PlayerRotateToMouseCursor();
		
		MontageJumpToSection(*FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
		GetAbilitySystemComponentFromActorInfo_Checked()->CancelAbilities(new FGameplayTagContainer(TAG_GAS_ATTACK_HIT_CHECK));
		bIsCanPlayNextAction = false;

		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]()
		{
			if(IsValid(WaitTagTask) && !bIsAbilityEnding)
			{
				WaitTagTask->Activate();
			}
		}));
		
		PlayerCharacter->SetAttacking(true);
		ExecuteAttackCueWithDelay();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckPlayerAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("다음 공격 액션 실행. 현재 몽타주 섹션 번호: %d"), CurrentComboAction));
			}
		}
#endif
	}
}

void ULLL_PGA_AttackBase::EndAttackInputWait()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void ULLL_PGA_AttackBase::ExecuteAttackCueWithDelay()
{
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if (AttackCueDelayArray.Num() >= 1)
	{
		int32 AttackEventDelayIndex = CurrentComboAction - 1;
		if (AttackCueDelayArray.Num() <= AttackEventDelayIndex)
		{
			AttackEventDelayIndex = AttackCueDelayArray.Num() - 1;
		}

		if (AttackCueDelayArray[AttackEventDelayIndex] > 0)
		{
			const UAnimInstance* PlayerAnimInstance = PlayerCharacter->GetCharacterAnimInstance();
			const float MontagePlayRate = PlayerAnimInstance->Montage_GetPlayRate(AttackAnimMontage);
			
			int32 CapturedCurrentComboAction = CurrentComboAction;
			FTimerHandle AttackEventDelayTimerHandle;

			GetWorld()->GetTimerManager().SetTimer(AttackEventDelayTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, CapturedCurrentComboAction]{
				FLLL_ExecuteCueHelper::ExecuteCue(PlayerCharacter, AttackCueTag);
				PlayerCharacter->GetFModAudioComponent()->SetParameter(PlayerAttackCountParameterName, CapturedCurrentComboAction - 1);
			}), AttackCueDelayArray[AttackEventDelayIndex] * MontagePlayRate, false);
		}
		else
		{
			FLLL_ExecuteCueHelper::ExecuteCue(PlayerCharacter, AttackCueTag);
			PlayerCharacter->GetFModAudioComponent()->SetParameter(PlayerAttackCountParameterName, CurrentComboAction - 1);
		}
	}
	else
	{
		FLLL_ExecuteCueHelper::ExecuteCue(PlayerCharacter, AttackCueTag);
		PlayerCharacter->GetFModAudioComponent()->SetParameter(PlayerAttackCountParameterName, CurrentComboAction - 1);
	}
}
