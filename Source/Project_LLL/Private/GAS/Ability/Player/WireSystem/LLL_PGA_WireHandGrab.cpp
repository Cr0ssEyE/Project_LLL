// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/Player/WireSystem/LLL_PGA_WireHandGrab.h"

#include "AbilitySystemComponent.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "GAS/Attribute/Player/LLL_PlayerWireHandAttributeSet.h"

ULLL_PGA_WireHandGrab::ULLL_PGA_WireHandGrab()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool ULLL_PGA_WireHandGrab::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	bool Result = Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	if(IsActive())
	{
		return false;
	}
	return Result;
}

void ULLL_PGA_WireHandGrab::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.AddDynamic(this, &ULLL_PGA_WireHandGrab::OnReleasedCallBack);
	PlayerWireHand->GetHandMesh()->SetRelativeScale3D(PlayerWireHand->GetHandMesh()->GetRelativeScale3D() * WireHandOnGrabbedScale);
	GrabElapsedTime = 0.f;
	MaxGrabDuration = PlayerWireHand->GetWireHandAttributeSet()->GetGrabDuration();
	GrabAroundEntity();
}

void ULLL_PGA_WireHandGrab::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->ReleaseCompleteDelegate.RemoveDynamic(this, &ULLL_PGA_WireHandGrab::OnReleasedCallBack);
	PlayerWireHand->GetHandMesh()->SetRelativeScale3D(PlayerWireHand->GetHandMesh()->GetRelativeScale3D() / WireHandOnGrabbedScale);
	
	const FGameplayTagContainer ReleaseHandTags(TAG_GAS_WIRE_RELEASE);
	GetAbilitySystemComponentFromActorInfo_Checked()->TryActivateAbilitiesByTag(ReleaseHandTags);
	GrabElapsedTime = 0.f;
	MaxGrabDuration = 0.f;
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void ULLL_PGA_WireHandGrab::GrabAroundEntity()
{
	ALLL_PlayerWireHand* PlayerWireHand = CastChecked<ALLL_PlayerWireHand>(CurrentActorInfo->AvatarActor);
	PlayerWireHand->GetHandMesh()->SetAnimation(GrabAnim);
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandGrab::CheckGrabbedTime);
}

void ULLL_PGA_WireHandGrab::CheckGrabbedTime()
{
	if(MaxGrabDuration <= 0.f)
	{
		return;
	}
	
	GrabElapsedTime += GetWorld()->GetDeltaSeconds();
	if(GrabElapsedTime >= MaxGrabDuration)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ULLL_PGA_WireHandGrab::CheckGrabbedTime);
}

void ULLL_PGA_WireHandGrab::OnReleasedCallBack()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
