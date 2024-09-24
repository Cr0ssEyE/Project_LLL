// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_RangeKnockBackDetectorDataAsset.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "LLL_RangeKnockBackDetector.generated.h"

class ULLL_RangeKnockBackDetectorAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_RangeKnockBackDetector : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_RangeKnockBackDetector();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> OverlapCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_RangeKnockBackDetectorDataAsset> RangeKnockBackDetectorDataAsset;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_RangeKnockBackDetectorAttributeSet> RangeKnockBackDetectorAttributeSet;
};
