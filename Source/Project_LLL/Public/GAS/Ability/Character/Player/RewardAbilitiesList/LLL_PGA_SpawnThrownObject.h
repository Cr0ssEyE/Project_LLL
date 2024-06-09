// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "LLL_PGA_SpawnThrownObject.generated.h"

class ALLL_ThrownObject;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_SpawnThrownObject : public ULLL_PGA_RewardAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UPROPERTY(EditAnywhere, DisplayName = "스폰 투사체 클래스")
	TSubclassOf<ALLL_ThrownObject> ThrownObjectClass;

	UPROPERTY(EditAnywhere, DisplayName = "속도")
	float Speed;

	UPROPERTY(EditAnywhere, DisplayName = "플레이어가 피격됐을 때 발동")
	uint8 bTargetIsInstigator : 1;

	UPROPERTY(EditAnywhere, DisplayName = "스폰 간격 시간")
	float SpawnOffsetTime;
};
