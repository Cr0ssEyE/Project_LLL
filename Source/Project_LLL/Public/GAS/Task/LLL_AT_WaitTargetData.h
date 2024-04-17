// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LLL_AT_WaitTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTargetDataReceivedDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AT_WaitTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:
	ULLL_AT_WaitTargetData();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULLL_AT_WaitTargetData* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<AActor> RequiredClass, bool AllTargetHasMatching = false, bool UsingOnce = true);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

public:
	UPROPERTY(BlueprintCallable)
	FTargetDataReceivedDelegate TargetDataReceivedDelegate;
	
protected:
	UFUNCTION(BlueprintCallable)
	void OnTargetDataReceivedCallBack(const FGameplayAbilityTargetDataHandle& TargetDataHandle);

protected:
	UPROPERTY()
	TSubclassOf<AActor> RequiredActorClass;

	uint8 bUsingOnce : 1;

	uint8 bAllTargetHasMatching : 1;
};
