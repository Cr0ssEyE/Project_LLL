// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_ChaseHandThrow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ChaseHandThrow : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_ChaseHandThrow();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual void ThrowToCursorLocation();

	virtual void CheckReached();

protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "체이서의 발사 상태 애니메이션")
	TObjectPtr<UAnimSequence> ThrowAnim;

	FVector StartLocation;
	
	FVector TargetLocation;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "체이서 투척 이벤트 태그", meta=(Categories = "GameplayCue"))
	FGameplayTag WireHandThrowCueTag;
};
