// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/LLL_PGA_GroundStrike.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLL_ExecuteCueHelper.h"

ULLL_PGA_GroundStrike::ULLL_PGA_GroundStrike()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_GroundStrike::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerSkillDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("스킬 어빌리티 발동")));
		}
	}
#endif

	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(!IsValid(Player))
	{
		return;
	}

	if(!IsValid(AbilityActionMontage))
	{
		return;
	}

	if(Player->GetCharacterMovement()->IsWalking())
	{
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), AbilityActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_GroundStrike::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_GroundStrike::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
	
	FLLL_ExecuteCueHelper::ExecuteCue(Player, WireAttackCueTag);
}

void ULLL_PGA_GroundStrike::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerSkillDebug())
		{
			if(bWasCancelled)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("스킬 어빌리티 중단됨")));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("스킬 어빌리티 종료")));
			}
		}
	}
#endif

	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
