﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_Skill_BulletTime.generated.h"

class ALevelSequenceActor;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_Skill_BulletTime : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_Skill_BulletTime();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UFUNCTION()
	void TraceBulletTimeEffectedActors();

	UFUNCTION()
	void BulletTimeEndedCallBack();

	UFUNCTION()
	void OnBulletTimeEffectedActorSpawnCheck(AActor* Actor);
	
	// 효과 관련
protected:
	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "불릿타임 효과 발동 시퀀스")
	TObjectPtr<ULevelSequence> BulletTimeActivateSequence;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "불릿타임 효과 종료 시퀀스")
	TObjectPtr<ULevelSequence> BulletTimeDeActivateSequence;

	UPROPERTY(EditAnywhere, Category = "GAS", DisplayName = "불릿타임 효과 종료 넉백 충돌체크 어빌리티 태그")
	FGameplayTag KnockBackCollideCheckTag;
	
protected:
	UPROPERTY()
	TObjectPtr<ALevelSequenceActor> BulletTimeActivateSequenceActor;

	UPROPERTY()
	TObjectPtr<ALevelSequenceActor> BulletTimeDeActivateSequenceActor;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> BulletTimeEffectedActors;

	FOnActorSpawned::FDelegate WorldActorSpawnedDelegate;

	FDelegateHandle ActorSpawnedDelegateHandle;
	
protected:
	FTimerHandle AbilityDurationTimerHandle;
	
	float SkillDuration;
	float WorldDecelerationRate;
	float PlayerAccelerationRate;
};
