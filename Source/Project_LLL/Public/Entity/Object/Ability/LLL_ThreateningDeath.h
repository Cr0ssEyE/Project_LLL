// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/LLL_AbilityObject.h"
#include "DataAsset/LLL_ThreateningDeathDataAsset.h"
#include "LLL_ThreateningDeath.generated.h"

class ULLL_ThreateningDeathAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ThreateningDeath : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_ThreateningDeath();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ThreateningDeathDataAsset> ThreateningDeathDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ThreateningDeathAttributeSet> ThreateningDeathAttributeSet;
 };
