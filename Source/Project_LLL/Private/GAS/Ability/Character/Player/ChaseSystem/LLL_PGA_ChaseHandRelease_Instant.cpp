// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Character/Player/ChaseSystem/LLL_PGA_ChaseHandRelease_Instant.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"

ULLL_PGA_ChaseHandRelease_Instant::ULLL_PGA_ChaseHandRelease_Instant()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_ChaseHandRelease_Instant::RetargetReleaseVelocity()
{
	ALLL_PlayerChaseHand* PlayerChaseHand = CastChecked<ALLL_PlayerChaseHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerChaseHand->GetOwner());
	PlayerChaseHand->TeleportTo(PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorRotation());

	if (CheckOwnerAlreadyOverlapped())
	{
		PlayerChaseHand->SetHiddenState();
		PlayerChaseHand->ReleaseCompleteDelegate.Broadcast();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

