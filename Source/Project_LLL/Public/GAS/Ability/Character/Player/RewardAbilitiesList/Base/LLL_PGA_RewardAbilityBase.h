// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_RewardAbilityBase.generated.h"

/**
 * 
 */

struct FAbilityDataTable;
enum class EAbilityRank : uint8;

UCLASS()
class PROJECT_LLL_API ULLL_PGA_RewardAbilityBase : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetAbilityInfo(const FAbilityDataTable* InAbilityData) { AbilityData = InAbilityData; }

	FORCEINLINE const FAbilityDataTable* GetAbilityData() const { return AbilityData; }
	
protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	const FAbilityDataTable* AbilityData;
};
