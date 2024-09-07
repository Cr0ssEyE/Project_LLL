// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MonsterGameplayAbilityBase.h"
#include "LLL_MGA_SpawnThrownObject.generated.h"

class ALLL_ThrownObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_MGA_SpawnThrownObject : public ULLL_MonsterGameplayAbilityBase
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, Category = "GAS", DisplayName = "투사체 클래스")
	TSubclassOf<ALLL_ThrownObject> ThrownObjectClass;
};
