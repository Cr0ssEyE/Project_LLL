// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_PGA_WireHandRelease.h"
#include "LLL_PGA_WireHandRelease_Instant.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PGA_WireHandRelease_Instant : public ULLL_PGA_WireHandRelease
{
	GENERATED_BODY()

public:
	ULLL_PGA_WireHandRelease_Instant();

protected:
	virtual void RetargetReleaseVelocity() override;
};
