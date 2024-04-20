// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Base/LLL_CharacterGameplayAbilityBase.h"
#include "LLL_MonsterGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MonsterGameplayAbilityBase : public ULLL_CharacterGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
};
