// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LLL_AT_Trace.generated.h"

class ALLL_TA_TraceBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskOnCompleteDelegate, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AT_Trace : public UAbilityTask
{
	GENERATED_BODY()

public:
	ULLL_AT_Trace();

	// bIsTraceSingleTick이 false인 경우: OwningAbility가 종료되거나 따로 EndTask를 호출하기 전 까지 Task가 유지됨
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULLL_AT_Trace* CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<ALLL_TA_TraceBase> TargetActorClass, bool TraceSingleTick = true);
	
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;
	
public:
	UPROPERTY(BlueprintCallable)
	FTaskOnCompleteDelegate TaskOnCompleteDelegate;

protected:
	void SpawnAndInitializeTargetActor();
	void FinalizeTargetActor();

protected:
	void OnTraceCompletedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle);

protected:
	UPROPERTY()
	TSubclassOf<ALLL_TA_TraceBase> TargetActorClass;

	UPROPERTY()
	TObjectPtr<ALLL_TA_TraceBase> SpawnedTargetActor;

	UPROPERTY()
	TSet<AActor*> IgnoreActors;

	uint8 bIsTraceSingleTick : 1;
};
