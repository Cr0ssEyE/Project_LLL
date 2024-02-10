// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API ULLL_PGA_Dash : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_Dash();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	void DashActionEvent();
	void StartDashInputDelay();
	void EndDashInputDelay();
	
protected:
	UPROPERTY(EditAnywhere, DisplayName = "대쉬 애님 몽타주")
	TObjectPtr<UAnimMontage> DashAnimMontage;
	
	UPROPERTY(EditAnywhere, DisplayName = "대쉬 연속 사용 제한시간")
	float DashInputCheckTime = 0.f;
	
	FTimerHandle DashInputDelayHandle;
	
	uint32 CurrentDashCount;

	uint32 MaxDashCount;

	uint32 DashSpeed;

	uint32 bIsInputPressed : 1;
};
