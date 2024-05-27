// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Util/LLL_ExecuteCueHelper.h"

ULLL_PGA_ChaseAttack::ULLL_PGA_ChaseAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_ChaseAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(GetAvatarActorFromActorInfo());
	if(!IsValid(PlayerCharacter))
	{
		return;
	}

	if(!IsValid(AbilityActionMontage))
	{
		return;
	}

	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = Cast<ULLL_PlayerCharacterAttributeSet>(GetAbilitySystemComponentFromActorInfo_Checked()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
	
	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ChaseMontage"), AbilityActionMontage, PlayerCharacterAttributeSet->GetAttackSpeed(), SECTION_ATTACK);
	PlayMontageTask->OnCompleted.AddDynamic(this, &ULLL_PGA_ChaseAttack::OnCompleteCallBack);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_ChaseAttack::OnInterruptedCallBack);

	PlayMontageTask->ReadyForActivation();
}

void ULLL_PGA_ChaseAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
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
