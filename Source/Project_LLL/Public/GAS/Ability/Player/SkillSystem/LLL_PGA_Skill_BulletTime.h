﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_Skill_BulletTime.generated.h"

class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_Skill_BulletTime : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void PlayerActorAssignedCallBack(AActor* Actor);
	
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "불릿타임 효과 발동 시퀀스")
	TObjectPtr<ULevelSequence> BulletTimeActivateSequence;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "불릿타임 효과 종료 시퀀스")
	TObjectPtr<ULevelSequence> BulletTimeDeActivateSequence;

	float SkillDuration;
	float WorldDecelerationRate;
	float PlayerAccelerationRate;
};
