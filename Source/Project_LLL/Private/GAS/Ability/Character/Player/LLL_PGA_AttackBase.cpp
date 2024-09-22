// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AnimNotify_PlayNiagaraEffect.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_AnimRelationNames.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Particles/ParticleSystemComponent.h"

ULLL_PGA_AttackBase::ULLL_PGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboAction = 0;
	MaxAttackAction = 0;
	bIsCanPlayNextAction = false;
}

void ULLL_PGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("공격 어빌리티 발동")));
		}
	}
#endif
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	if(IsValid(Player) && IsValid(PlayerAttributeSet) && IsValid(AttackAnimMontage))
	{
		MaxAttackAction = PlayerAttributeSet->GetMaxAttackActionCount();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}

	Player->RotateToMouseCursor();

	FGameplayTagContainer OwnedTagsContainer;
	GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags(OwnedTagsContainer);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackAnimMontage, PlayerAttributeSet->GetAttackSpeed(), *FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
	MontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_AttackBase::OnCompleteCallBack);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_AttackBase::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();

	WaitTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_STATE_INPUT_CHECK_ATTACK, nullptr, true);
	WaitTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_AttackBase::CheckInputPressed);
	WaitTagTask->ReadyForActivation();

	Player->SetAttacking(true);
	Player->SetCurrentCombo(CurrentComboAction);
}

void ULLL_PGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
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
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(Player))
	{
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurrentComboAction = 0;
		bIsCanPlayNextAction = false;
		Player->SetAttacking(false);
		Player->SetCurrentCombo(CurrentComboAction);
	}
	
	if (bWasCancelled)
	{
		for (auto Notify : AttackAnimMontage->Notifies)
		{
			UAnimNotify_PlayNiagaraEffect* NiagaraEffectNotify = Cast<UAnimNotify_PlayNiagaraEffect>(Notify.Notify);
			if (!NiagaraEffectNotify)
			{
				continue;
			}

			UFXSystemComponent* NotifyComponent = NiagaraEffectNotify->GetSpawnedEffect();
			if (IsValid(GetWorld()) && IsValid(Player) && IsValid(NotifyComponent) && !NotifyComponent->IsGarbageEliminationEnabled())
			{
				NotifyComponent->DestroyComponent();
			}
		}
	}
	
	GetAbilitySystemComponentFromActorInfo_Checked()->CancelAbilities(new FGameplayTagContainer(TAG_GAS_ATTACK_HIT_CHECK));
	WaitTagTask->EndTask();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_AttackBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
	{
		ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
		PlayerCharacter->CheckMouseLocation();
		bIsCanPlayNextAction = true;
	}
}

void ULLL_PGA_AttackBase::CheckInputPressed(FGameplayEventData EventData)
{
	if (!bIsCanPlayNextAction)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	SetNextAttackAction();
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("공격 추가 입력 인식")));
		}
	}
#endif
}

void ULLL_PGA_AttackBase::SetNextAttackAction()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const UAnimInstance* PlayerAnimInstance = Player->GetCharacterAnimInstance();
	if(IsValid(Player) && bIsCanPlayNextAction && PlayerAnimInstance->Montage_IsPlaying(AttackAnimMontage))
	{
		FGameplayTagContainer OwnedTagsContainer;
		GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags(OwnedTagsContainer);
		
		if(CurrentComboAction == MaxAttackAction)
		{
			CurrentComboAction = 0;
		}
		
		Player->RotateToMouseCursor(1.f, true);

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActorFromActorInfo(), TAG_GAS_ATTACK_HIT_CHECK_COMPLETE, FGameplayEventData());
		MontageJumpToSection(*FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
		bIsCanPlayNextAction = false;

		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]()
		{
			if(!bIsAbilityEnding)
			{
				WaitTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_STATE_INPUT_CHECK_ATTACK, nullptr, true);
				WaitTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_AttackBase::CheckInputPressed);
				WaitTagTask->ReadyForActivation();
			}
		}));
		
		Player->SetAttacking(true);
		Player->SetCurrentCombo(CurrentComboAction);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("다음 공격 액션 실행. 현재 몽타주 섹션 번호: %d"), CurrentComboAction));
			}
		}
#endif
	}
}