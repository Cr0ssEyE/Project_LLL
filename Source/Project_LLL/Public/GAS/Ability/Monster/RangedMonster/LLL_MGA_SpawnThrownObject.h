// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_SpawnThrownObject.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_SpawnThrownObject : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
