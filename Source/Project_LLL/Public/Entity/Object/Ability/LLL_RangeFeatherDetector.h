// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_RangeFeatherDetectorDataAsset.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "LLL_RangeFeatherDetector.generated.h"

class ULLL_RangeFeatherDetectorAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_RangeFeatherDetector : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_RangeFeatherDetector();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void Destroyed() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_RangeFeatherDetectorDataAsset> RangeFeatherDetectorDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_RangeFeatherDetectorAttributeSet> RangeFeatherDetectorAttributeSet;
};
