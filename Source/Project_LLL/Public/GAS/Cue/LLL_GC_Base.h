// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Burst.h"
#include "LLL_GC_Base.generated.h"

class UFMODEvent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GC_Base : public UGameplayCueNotify_Burst
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "FMOD")
	TObjectPtr<UFMODEvent> FModEvent;
};
