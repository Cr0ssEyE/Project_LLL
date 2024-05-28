// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ControlChaseHand.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Thrown/LLL_PlayerChaseHandAttributeSet.h"

ULLL_PGA_ControlChaseHand::ULLL_PGA_ControlChaseHand()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bIsAlreadyThrown = false;
}

void ULLL_PGA_ControlChaseHand::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();
	
	PlayerChaseHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_ControlChaseHand::OnCompleteCallBack);

	if (!IsValid(ThrowAnimMontage))
	{
		EndAbility(CurrentSpecHandle,  CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("ThrowMontage"), ThrowAnimMontage, 1.0f);
	MontageTask->OnInterrupted.AddDynamic(this, &ULLL_PGA_ControlChaseHand::OnInterruptedCallBack);
	MontageTask->ReadyForActivation();
	
	UAbilityTask_WaitGameplayEvent* ThrowTriggerTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TAG_GAS_PLAYER_THROW_TRIGGERED, nullptr, true);
	ThrowTriggerTask->EventReceived.AddDynamic(this, &ULLL_PGA_ControlChaseHand::ThrowHand);
	ThrowTriggerTask->ReadyForActivation();

	PlayerCharacter->PlayerRotateToMouseCursor(2.f);
}

void ULLL_PGA_ControlChaseHand::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();

	PlayerCharacter->GetMesh()->UnHideBoneByName(BONE_PLAYER_LEFT_WEAPON);
	PlayerChaseHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_ControlChaseHand::OnCompleteCallBack);

	if (PlayerChaseHand->GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_GAS_CHASER_STATE_THROWING))
	{
		PlayerChaseHand->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_RELEASE));
	}
	bIsAlreadyThrown = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_ControlChaseHand::ThrowHand(const FGameplayEventData EventData)
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
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerChaseHand* PlayerChaseHand = PlayerCharacter->GetChaseHand();

	UAbilitySystemComponent* HandASC = PlayerChaseHand->GetAbilitySystemComponent();
	if(!IsValid(HandASC))
	{
		EndAbility(CurrentSpecHandle,  CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	PlayerCharacter->GetMesh()->HideBoneByName(BONE_PLAYER_LEFT_WEAPON, PBO_Term);

	const ULLL_PlayerChaseHandAttributeSet* ChaseHandAttributeSet = Cast<ULLL_PlayerChaseHandAttributeSet>(PlayerChaseHand->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerChaseHandAttributeSet::StaticClass()));
	PlayerChaseHand->GetCollisionComponent()->SetSphereRadius(ChaseHandAttributeSet->GetGrabCollisionRadius());
	
	HandASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASER_THROW));
	bIsAlreadyThrown = true;
}
