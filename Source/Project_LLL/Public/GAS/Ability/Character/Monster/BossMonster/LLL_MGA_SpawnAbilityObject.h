// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_SpawnAbilityObject.generated.h"

class ALLL_AbilityObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_SpawnAbilityObject : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "어빌리티 오브젝트 클래스")
	TSubclassOf<ALLL_AbilityObject> AbilityObjectClass;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "생성 위치")
	EEffectApplyTarget AbilityObjectLocationTarget;
};
