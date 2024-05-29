// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_AbilityObject.h"
#include "DataAsset/LLL_PlayerThreateningDeathDataAsset.h"
#include "LLL_PlayerThreateningDeath.generated.h"

class ULLL_PlayerThreateningDeathAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_PlayerThreateningDeath : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_PlayerThreateningDeath();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_PlayerThreateningDeathDataAsset> PlayerThreateningDeathDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_PlayerThreateningDeathAttributeSet> PlayerThreateningDeathAttributeSet;
 };
