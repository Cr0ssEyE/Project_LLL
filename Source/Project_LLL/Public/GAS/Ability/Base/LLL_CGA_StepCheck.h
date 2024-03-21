// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Base/LLL_CharacterGameplayAbilityBase.h"
#include "LLL_CGA_StepCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CGA_StepCheck : public ULLL_CharacterGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
