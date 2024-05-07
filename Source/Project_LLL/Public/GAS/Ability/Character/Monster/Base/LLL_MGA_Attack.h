// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_Attack : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()

public:
	UAnimMontage* GetAttackMontage() const { return AttackMontage; }

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
};
