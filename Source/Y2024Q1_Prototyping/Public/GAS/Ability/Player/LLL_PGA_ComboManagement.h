// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ComboManagement.generated.h"

class ULLL_PlayerAttributeSet;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PGA_ComboManagement : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ComboManagement();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void ComboTimerTick();
	
protected:
	UPROPERTY()
	TObjectPtr<const ULLL_PlayerAttributeSet> PlayerAttributes;

	uint32 CurrentComboCount;
	
	float CurrentComboStackDuration;
	
	float MaxComboStackDuration;
};
