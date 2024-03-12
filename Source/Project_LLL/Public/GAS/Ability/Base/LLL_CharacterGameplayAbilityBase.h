// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Enumeration/LLL_EffectApplyTargetHelper.h"
#include "LLL_CharacterGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_CharacterGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnCompleteCallBack();
	
	UFUNCTION(BlueprintCallable)
	virtual void OnInterruptedCallBack();
	
protected:
	UPROPERTY(EditAnywhere, Category = "GameplayEffect", DisplayName = "활성화시 호출되는 이펙트 리스트")
	TMap<TSubclassOf<UGameplayEffect>, EEffectApplyTarget> OnActivateEffects;
	
	UPROPERTY(EditAnywhere, Category = "GameplayEffect", DisplayName = "종료시 호출되는 이펙트 리스트")
	TMap<TSubclassOf<UGameplayEffect>, EEffectApplyTarget> OnEndedEffects;
};
