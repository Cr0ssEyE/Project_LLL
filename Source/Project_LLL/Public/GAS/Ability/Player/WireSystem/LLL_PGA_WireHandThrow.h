﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_WireHandThrow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_WireHandThrow : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_WireHandThrow();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void ThrowToCursorLocation();

	virtual void CheckReached();

protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "와이어의 투척 애니메이션")
	TObjectPtr<UAnimSequence> ThrowAnim;

	FVector TargetLocation;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "와이어 투척 이벤트 태그", meta=(Categories = "GameplayCue"))
	FGameplayTag WireHandThrowCueTag;
};
