// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/Base/LLL_SystemBase.h"
#include "LLL_Debug_BossSpawner.generated.h"

/**
 * 
 */
class ALLL_ManOfStrength;
class UBoxComponent;
class ALLL_ManOfStrength;
UCLASS()
class PROJECT_LLL_API ALLL_Debug_BossSpawner : public ALLL_SystemBase
{
	GENERATED_BODY()

	ALLL_Debug_BossSpawner();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY()
	TSubclassOf<ALLL_ManOfStrength> BossClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DetectBox;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ALLL_ManOfStrength> Boss;
};
