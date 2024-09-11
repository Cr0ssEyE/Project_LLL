// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_Charge.generated.h"

enum class EBossMonsterPattern : uint8;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_Charge : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta=(EditCondition = "bIsBossMonster == false", EditConditionHides))
	TObjectPtr<UAnimMontage> ChargeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation", meta=(EditCondition = "bIsBossMonster == true", EditConditionHides))
	TMap<EBossMonsterPattern, TObjectPtr<UAnimMontage>> ChargeMontages;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	uint8 bIsBossMonster : 1;
};
