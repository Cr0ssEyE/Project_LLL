// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Character/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_Dash.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_MoveToLocation;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_Dash : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_Dash();

	FORCEINLINE UAnimMontage* GetDashAnimMontage() const { return DashAnimMontage; }
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	void DashActionEvent();
	
	UFUNCTION()
	void LocationReachedEvent();
	
	UFUNCTION()
	void CheckInputPressed(FGameplayEventData EventData);

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_MoveToLocation> DashTask;
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "대쉬 애님 몽타주")
	TObjectPtr<UAnimMontage> DashAnimMontage;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitTagTask;
	
	uint32 CurrentDashCount;

	uint32 MaxDashCount;

	float DashSpeed;

	float DashDistance;

	float DashCorrectionDistance;
	
	uint32 bIsInputPressed : 1;

protected:
	UPROPERTY(EditAnywhere, DisplayName = "대시 이벤트 태그", meta=(Categories = "GameplayCue"))
	FGameplayTag DashCueTag;
};
