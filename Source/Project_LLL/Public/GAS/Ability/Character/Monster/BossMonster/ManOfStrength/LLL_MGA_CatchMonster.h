﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Base/LLL_CharacterGameplayAbilityBase.h"
#include "LLL_MGA_CatchMonster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_CatchMonster : public ULLL_CharacterGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "소켓 이름")
	FName SocketName;
};
