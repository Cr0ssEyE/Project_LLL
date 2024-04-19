// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PGA_ChaseHandRelease.h"
#include "LLL_PGA_ChaseHandRelease_Instant.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_ChaseHandRelease_Instant : public ULLL_PGA_ChaseHandRelease
{
	GENERATED_BODY()

public:
	ULLL_PGA_ChaseHandRelease_Instant();

protected:
	virtual void RetargetReleaseVelocity() override;
};
