// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ComboManagement.generated.h"

class ULLL_PlayerCharacterAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ComboManagement : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ComboManagement();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void ComboTimerTick();

protected:
	UPROPERTY()
	TObjectPtr<const ULLL_PlayerCharacterAttributeSet> PlayerAttributes;

	UPROPERTY(EditDefaultsOnly, Category = "GameplayEffect", DisplayName = "콤보 타이머 경과시 호출되는 이펙트")
	TSubclassOf<UGameplayEffect> ComboDivideEffect;
	
	uint32 CurrentComboCount;
	
	float CurrentComboStackDuration;
	
	float MaxComboStackDuration;
};
