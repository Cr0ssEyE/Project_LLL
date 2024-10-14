// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/LLL_PGA_AttackBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AnimNotify_PlayNiagaraEffect.h"
#include "Abilities/Tasks/AbilityTask_MoveToLocation.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AnimNotify/LLL_AnimNotify_GameplayTag.h"
#include "Constant/LLL_AnimRelationNames.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
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

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	
	// 과충전 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_CHARGE_ATTACK))
	{
		ChargeAttack();
	}
	else
	{
		BaseAttack();
	}
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
	if (IsValid(WaitTagTask))
	{
		WaitTagTask->EndTask();
	}

	bStopCharge = false;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_AttackBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if (IsValid(WaitTagTask) && WaitTagTask->IsActive())
	{
		Player->CheckMouseLocation();
		bIsCanPlayNextAction = true;
	}

	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	
	// 과충전 이누리아
	if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_CHARGE_ATTACK) && !bStopCharge)
	{
		bStopCharge = true;

		ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(Player->GetCharacterAnimInstance());
		const float MontagePosition = PlayerAnimInstance->Montage_GetPosition(ChargeAttackAnimMontage);
		const float ChargeRate = MontagePosition / GetFullChargeNotifyTriggerTime();
		UE_LOG(LogTemp, Log, TEXT("차지 비율 : %f"), ChargeRate);
		Player->SetChargeAttackChargeRate(ChargeRate);
		
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		float AttackSpeed = PlayerAttributeSet->GetAttackSpeed();
		AttackSpeed *= PlayerAttributeSet->GetFasterAttackAttackSpeedRate();

		UAbilityTask_PlayMontageAndWait* ChargeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ChargeAttackMontage"), ChargeAttackAnimMontage, AttackSpeed, NAME_None, true, 1, GetFullChargeNotifyTriggerTime());
		ChargeMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_AttackBase::OnCompleteCallBack);
		ChargeMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_AttackBase::OnInterruptedCallBack);
		ChargeMontageTask->ReadyForActivation();
		
		FVector Location = Player->GetActorLocation();
		const float OffsetAttackRange = PlayerAttributeSet->GetMaxChargeAttackRange() - PlayerAttributeSet->GetMinChargeAttackRange();
		const float TempAttackRange = PlayerAttributeSet->GetMinChargeAttackRange() + Player->GetChargeAttackChargeRate() * OffsetAttackRange;
		Location += Player->GetActorForwardVector() * TempAttackRange;

		const float OffsetDuration = PlayerAttributeSet->GetMaxChargeAttackDuration() - PlayerAttributeSet->GetMinChargeAttackDuration();
		const float Duration = PlayerAttributeSet->GetMinChargeAttackDuration() + Player->GetChargeAttackChargeRate() * OffsetDuration;
		
		UAbilityTask_MoveToLocation* MoveTask = UAbilityTask_MoveToLocation::MoveToLocation(this, FName("Move"), Location, Duration, nullptr, nullptr);
		MoveTask->ReadyForActivation();
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

void ULLL_PGA_AttackBase::CheckFullCharge(FGameplayEventData EventData)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	ULLL_PlayerAnimInstance* PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(Player->GetCharacterAnimInstance());
	PlayerAnimInstance->Montage_Pause(ChargeAttackAnimMontage);
	PlayerAnimInstance->Montage_SetPosition(ChargeAttackAnimMontage, GetFullChargeNotifyTriggerTime());
}

void ULLL_PGA_AttackBase::BaseAttack()
{
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

	float AttackSpeed = PlayerAttributeSet->GetAttackSpeed();
	AttackSpeed *= PlayerAttributeSet->GetFasterAttackAttackSpeedRate();
	
	const FName Section = *FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction);
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackAnimMontage, AttackSpeed, Section);
	MontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_AttackBase::OnCompleteCallBack);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_AttackBase::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();

	WaitTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_STATE_INPUT_CHECK_ATTACK, nullptr, true);
	WaitTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_AttackBase::CheckInputPressed);
	WaitTagTask->ReadyForActivation();

	Player->SetAttacking(true);
	Player->SetCurrentCombo(CurrentComboAction);
}

void ULLL_PGA_AttackBase::SetNextAttackAction()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const UAnimInstance* PlayerAnimInstance = Player->GetCharacterAnimInstance();
	if(IsValid(Player) && bIsCanPlayNextAction && PlayerAnimInstance->Montage_IsPlaying(AttackAnimMontage))
	{
		FGameplayTagContainer OwnedTagsContainer;
		GetAbilitySystemComponentFromActorInfo_Checked()->GetOwnedGameplayTags(OwnedTagsContainer);
		
		if (CurrentComboAction == MaxAttackAction)
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

void ULLL_PGA_AttackBase::ChargeAttack()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	ChargeRotate(Player);

	float AttackSpeed = GetFullChargeNotifyTriggerTime() / PlayerAttributeSet->GetMaxChargeAttackChargingTime();
	AttackSpeed *= PlayerAttributeSet->GetAttackSpeed();
	AttackSpeed *= PlayerAttributeSet->GetFasterAttackAttackSpeedRate();

	Player->PlayAnimMontage(ChargeAttackAnimMontage, AttackSpeed);
	Player->GetCharacterMovement()->SetMovementMode(MOVE_None);

	UAbilityTask_WaitGameplayEvent* WaitChargeTagTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_FULL_CHARGE_CHECK, nullptr, true);
	WaitChargeTagTask->EventReceived.AddDynamic(this, &ULLL_PGA_AttackBase::CheckFullCharge);
	WaitChargeTagTask->ReadyForActivation();
}

void ULLL_PGA_AttackBase::ChargeRotate(ALLL_PlayerBase* Player)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, Player]{
		Player->RotateToMouseCursor();
		if (bStopCharge)
		{
			return;
		}
		ChargeRotate(Player);
	}));
}

float ULLL_PGA_AttackBase::GetFullChargeNotifyTriggerTime() const
{
	for (const FAnimNotifyEvent& NotifyEvent : ChargeAttackAnimMontage->Notifies)
	{
		const ULLL_AnimNotify_GameplayTag* AnimNotify_GameplayTag = Cast<ULLL_AnimNotify_GameplayTag>(NotifyEvent.Notify);
		if (IsValid(AnimNotify_GameplayTag) && AnimNotify_GameplayTag->GetTriggerGameplayTag() == FGameplayTagContainer(TAG_GAS_FULL_CHARGE_CHECK))
		{
			return NotifyEvent.GetTriggerTime();
		}
	}

	return 0;
}
