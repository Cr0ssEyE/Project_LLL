// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ControlChaseHand.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"

ULLL_PGA_ControlChaseHand::ULLL_PGA_ControlChaseHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsAlreadyThrown = false;
}

void ULLL_PGA_ControlChaseHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();

	PlayerCharacter->GetMesh()->HideBoneByName(BONE_PLAYER_RIGHT_HAND, PBO_Term);
	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ControlChaseHand::OnCompleteCallBack);
	ThrowHand(ActorInfo);
}

void ULLL_PGA_ControlChaseHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();

	PlayerCharacter->GetMesh()->UnHideBoneByName(BONE_PLAYER_RIGHT_HAND);
	PlayerChaseHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ControlChaseHand::OnCompleteCallBack);
	
	bIsAlreadyThrown = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ControlChaseHand::ThrowHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(bIsAlreadyThrown)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerChaseActionDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("플레이어 와이어 투척이 이미 발동된 상태")));
			}
		}
#endif
		return;
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("플레이어 와이어 투척 어빌리티 발동")));
		}
	}
#endif
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();

	UAbilitySystemComponent* HandASC = PlayerChaseHand->GetAbilitySystemComponent();
	if(IsValid(HandASC))
	{
		const FGameplayTagContainer ThrowHandTags(TAG_GAS_CHASE_THROW);
		HandASC->TryActivateAbilitiesByTag(ThrowHandTags);
	}
	bIsAlreadyThrown = true;
}

