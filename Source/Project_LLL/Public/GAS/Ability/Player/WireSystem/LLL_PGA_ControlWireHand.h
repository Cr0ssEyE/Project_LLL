﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ControlWireHand.generated.h"

class ALLL_PlayerWireHand;
class ALLL_PlayerBase;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ControlWireHand : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ControlWireHand();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void ThrowHand(const FGameplayAbilityActorInfo* ActorInfo);
	virtual void ReleaseHand(const FGameplayAbilityActorInfo* ActorInfo);

	UFUNCTION(BlueprintCallable)
	virtual void OnReachedCallBack(FGameplayEventData Payload);
	
protected:
	UPROPERTY(EditDefaultsOnly, Meta=(Categories=Event))
	FGameplayTag GrabEventGameplayTag;

	UPROPERTY(EditDefaultsOnly, Meta=(Categories=Event))
	FGameplayTag ReachedEventGameplayTag;
	
	uint32 bIsAlreadyThrown : 1;
	
	uint32 bIsReleaseOnGoing : 1;
};
