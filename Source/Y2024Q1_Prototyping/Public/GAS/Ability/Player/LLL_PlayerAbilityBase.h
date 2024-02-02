// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LLL_PlayerAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PlayerAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	ULLL_PlayerAbilityBase();

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnCompleteCallBack();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnInterruptedCallback();

	
protected:
	UPROPERTY(EditAnywhere, Category = "GameplayEffect")
	TSubclassOf<UGameplayEffect> BaseEffect;

	UPROPERTY(EditAnywhere, Category = "GameplayEffect")
	TArray<TSubclassOf<UGameplayEffect>> AdditionalEffect;

	// UPROPERTY(EditAnywhere, Category = "TargetActor")
	
};
