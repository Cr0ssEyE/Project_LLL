// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_SpawnAbilityObject.generated.h"

class ALLL_AbilityObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_SpawnAbilityObject : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditAnywhere, DisplayName = "스폰 어빌리티 오브젝트 클래스")
	TSubclassOf<ALLL_AbilityObject> AbilityObjectClass;
};
