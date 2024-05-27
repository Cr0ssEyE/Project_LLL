// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_AbilityObject.h"
#include "DataAsset/LLL_PlayerThreateningDeathDataAsset.h"
#include "LLL_Player_ThreateningDeath.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_Player_ThreateningDeath : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_Player_ThreateningDeath();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_PlayerThreateningDeathDataAsset> ThreateningDeathDataAsset;
};
