// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_ControlWireHand.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerWireHand/LLL_PlayerWireHand.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/ThrownObject/PlayerWireHand/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_ControlWireHand::ULLL_PGA_ControlWireHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsAlreadyThrown = false;
}

void ULLL_PGA_ControlWireHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ControlWireHand::OnReleaseCompleteCallBack);
	ThrowHand(ActorInfo);
}

void ULLL_PGA_ControlWireHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();
	
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ControlWireHand::OnReleaseCompleteCallBack);
	
	bIsAlreadyThrown = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ControlWireHand::ThrowHand(const FGameplayAbilityActorInfo* ActorInfo)
{
	if(bIsAlreadyThrown)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerWireActionDebug())
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
		if(ProtoGameInstance->CheckPlayerWireActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("플레이어 와이어 투척 어빌리티 발동")));
		}
	}
#endif
	
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerWireHand* PlayerWireHand = PlayerCharacter->GetWireHand();

	UAbilitySystemComponent* HandASC = PlayerWireHand->GetAbilitySystemComponent();
	if(IsValid(HandASC))
	{
		const FGameplayTagContainer ThrowHandTags(TAG_GAS_WIRE_THROW);
		HandASC->TryActivateAbilitiesByTag(ThrowHandTags);
	}
	bIsAlreadyThrown = true;
}

void ULLL_PGA_ControlWireHand::OnReleaseCompleteCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
