// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_FeatherStormDataAsset.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "LLL_FeatherStorm.generated.h"

class ULLL_FeatherStormAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_FeatherStorm : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_FeatherStorm();

protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_FeatherStormDataAsset> FeatherStormDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_FeatherStormAttributeSet> FeatherStormAttributeSet;
	
	FTimerHandle KeepDamageHandle;
};
