// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandRelease_Instant.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"

ULLL_PGA_WireHandRelease_Instant::ULLL_PGA_WireHandRelease_Instant()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void ULLL_PGA_WireHandRelease_Instant::RetargetReleaseVelocity()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(PlayerWireHand->GetOwner());
	PlayerWireHand->TeleportTo(PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorRotation());

	if (CheckOwnerAlreadyOverlapped())
	{
		PlayerWireHand->SetHiddenState();
		PlayerWireHand->ReleaseCompleteDelegate.Broadcast();
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

