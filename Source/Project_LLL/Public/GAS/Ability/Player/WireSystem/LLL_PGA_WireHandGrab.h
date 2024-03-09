// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Ability/Player/LLL_PlayerGameplayAbilityBase.h"
#include "LLL_PGA_WireHandGrab.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_WireHandGrab : public ULLL_PlayerGameplayAbilityBase
{
	GENERATED_BODY()

public:
	ULLL_PGA_WireHandGrab();
	
protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	virtual bool TryGrabAroundEntity(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const;
	
	virtual void GrabTargetEntity();

	virtual void CheckGrabbedTime();

	UFUNCTION(BlueprintCallable)
	virtual void OnReleasedCallBack();
	
protected:
	UPROPERTY(EditDefaultsOnly, DisplayName = "와이어의 그랩 애니메이션")
	TObjectPtr<UAnimSequence> GrabAnim;

	UPROPERTY(EditDefaultsOnly, DisplayName = "그랩 대상에게 적용되는 이펙트")
	TSubclassOf<UGameplayEffect> GrabTargetApplyEffect;
	
protected:
	float GrabElapsedTime;

	float MaxGrabDuration;
	
};
