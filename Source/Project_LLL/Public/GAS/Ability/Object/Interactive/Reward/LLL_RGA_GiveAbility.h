﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Object/Interactive/Base/LLL_InteractiveObjectGameplayAbilityBase.h"
#include "LLL_RGA_GiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_RGA_GiveAbility : public ULLL_InteractiveObjectGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
