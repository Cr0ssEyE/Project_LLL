// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LLL_AT_WaitOverlap.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlapDelegate, const FGameplayAbilityTargetDataHandle&, TargetData);

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_AT_WaitOverlap : public UAbilityTask
{
	GENERATED_BODY()

public:
	ULLL_AT_WaitOverlap(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION()
	void OnOverlapCallBack(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void Activate() override;

	/** Wait until an overlap occurs. This will need to be better fleshed out so we can specify game specific collision requirements */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static ULLL_AT_WaitOverlap* WaitForOverlap(UGameplayAbility* OwningAbility, bool UsingOnce = true);

public:
	UPROPERTY(BlueprintAssignable)
	FOnOverlapDelegate OnOverlap;
	
private:
	virtual void OnDestroy(bool AbilityEnded) override;

	UPrimitiveComponent* GetComponent();

	uint8 bUsingOnce : 1;
};
