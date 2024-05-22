// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectComponents/AbilitiesGameplayEffectComponent.h"
#include "LLL_GE_GiveAbilityComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GE_GiveAbilityComponent : public UAbilitiesGameplayEffectComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE TArray<FGameplayAbilitySpecConfig>& GetAbilitySpecConfigs() { return GrantAbilityConfigs; }
	
};
