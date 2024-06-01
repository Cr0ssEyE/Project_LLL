// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_ChangeActionEffects.generated.h"

class UNiagaraSystem;/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ChangeActionEffects : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ChangeActionEffects();
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "교체 대상 애니메이션 몽타주")
	TObjectPtr<UAnimMontage> TargetAnimMontage;

	// 동일한 인덱스에 위치한 교체할 나이아가라 시스템 배열의 나이아가라 시스템과 교체됨
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "교체되는 나이아가라 시스템")
	TArray<TObjectPtr<UNiagaraSystem>> TargetNiagaraSystem;

	// 동일한 인덱스에 위치한 교체되는 나이아가라 시스템 배열의 나이아가라 시스템과 교체됨
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "교체할 나이아가라 시스템")
	TArray<TObjectPtr<UNiagaraSystem>> NewNiagaraSystem;

protected:
	uint8 bIsGenerated : 1;
	
};
