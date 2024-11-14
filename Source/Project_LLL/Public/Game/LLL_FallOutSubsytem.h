// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_FallOutSubsytem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_FallOutSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void FallOutBegin(AActor* Actor, FVector HitNormal, FVector HitLocation);
	void FallOutStart(AActor* Actor, FVector HitNormal);
};
