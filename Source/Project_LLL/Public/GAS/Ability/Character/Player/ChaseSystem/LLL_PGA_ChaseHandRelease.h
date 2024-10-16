﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ChaseHandRelease.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ChaseHandRelease : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	ULLL_PGA_ChaseHandRelease();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	virtual bool CheckOwnerAlreadyOverlapped();
	
	virtual void ReleaseToOwnerLocation();

	virtual void RetargetReleaseVelocity();

protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "체이서의 회수 애니메이션")
	TObjectPtr<UAnimSequence> ReleaseAnim;
	
};
