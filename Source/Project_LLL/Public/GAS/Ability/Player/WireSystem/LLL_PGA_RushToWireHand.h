﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_RushToWireHand.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_RushToWireHand : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_RushToWireHand();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	virtual void OwnerLaunchToWireHand();

	UFUNCTION(BlueprintCallable)
	virtual void OnReleasedCallBack();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "와이어 돌진 정지 거리")
	float AbilityEndDistance = 200.f;
	
	FVector TargetLocation;
	
	FVector Direction;

	float RushSpeed;
};
