// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_AttackBase.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"

ULLL_PGA_AttackBase::ULLL_PGA_AttackBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	CurrentComboAction = 0;
	ComboActionIntervalTime = 0.f;
	ComboActionInputDelayTime = 0.f;
	bIsInputPressed = false;
}

void ULLL_PGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	const ULLL_PlayerAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerAttributeSet>(PlayerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerAttributeSet::StaticClass()));
	if(IsValid(PlayerCharacter) && IsValid(PlayerAttributeSet) && IsValid(AttackAnimMontage))
	{
		MaxComboAction = PlayerAttributeSet->GetMaxComboAction();
		ComboActionIntervalTime = PlayerAttributeSet->GetComboActionIntervalTime();
		ComboActionInputDelayTime = PlayerAttributeSet->GetComboActionInputDelayTime();
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("AttackMontage"), AttackAnimMontage, 1.f, *FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
	MontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_AttackBase::OnCompleteCallBack);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_AttackBase::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();
	
	StartAttackInputWait();
}

void ULLL_PGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		CurrentComboAction = 0;
		bIsInputPressed = false;
		GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
		WaitInputTimerHandle.Invalidate();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_AttackBase::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
	float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(WaitInputTimerHandle);
	if(GetWorld()->GetTimerManager().IsTimerActive(WaitInputTimerHandle) && ElapsedTime > ComboActionInputDelayTime)
	{
		bIsInputPressed = true;
		SetNextAttackAction();
	}
}

void ULLL_PGA_AttackBase::SetNextAttackAction()
{
	GetWorld()->GetTimerManager().ClearTimer(WaitInputTimerHandle);
	WaitInputTimerHandle.Invalidate();
	ALLL_PlayerBase * PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(IsValid(PlayerCharacter) && bIsInputPressed && CurrentComboAction < MaxComboAction)
	{
		MontageJumpToSection(*FString::Printf(TEXT("%s%d"), SECTION_ATTACK, ++CurrentComboAction));
		StartAttackInputWait();
		bIsInputPressed = false;
	}
}

void ULLL_PGA_AttackBase::StartAttackInputWait()
{
	GetWorld()->GetTimerManager().SetTimer(WaitInputTimerHandle, this, &ULLL_PGA_AttackBase::EndAttackInputWait, ComboActionIntervalTime);
}

void ULLL_PGA_AttackBase::EndAttackInputWait()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
