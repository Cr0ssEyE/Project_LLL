// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_TA_TraceBase.h"
#include "LLL_TA_SweepMultiTrace.generated.h"

UCLASS()
class Y2024Q1_PROTOTYPING_API ALLL_TA_SweepMultiTrace : public ALLL_TA_TraceBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_TA_SweepMultiTrace();

protected:
	virtual FGameplayAbilityTargetDataHandle TraceResult() const override;
	
};
