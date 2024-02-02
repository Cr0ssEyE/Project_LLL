// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_GroundStrike.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"

ULLL_PGA_GroundStrike::ULLL_PGA_GroundStrike()
{
	
}

void ULLL_PGA_GroundStrike::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(ActorInfo->AvatarActor.Get());
	if(!IsValid(PlayerCharacter))
	{
		return;
	}

	if(!IsValid(AbilityActionMontage))
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), AbilityActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_GroundStrike::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_GroundStrike::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
}

void ULLL_PGA_GroundStrike::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(ActorInfo->AvatarActor.Get());
	if (PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_GroundStrike::OnCompleteCallBack()
{
	
	Super::OnCompleteCallBack();
}

void ULLL_PGA_GroundStrike::OnInterruptedCallBack()
{
	
	Super::OnInterruptedCallBack();
}
