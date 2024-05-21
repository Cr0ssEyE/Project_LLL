// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "LLL_AbilityObject.generated.h"

class ULLL_AbilityObjectAttributeSet;
class ULLL_AbilityObjectDataAsset;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_AbilityObject : public ALLL_BaseObject
{
	GENERATED_BODY()

public:
	ALLL_AbilityObject();

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> OverlapCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_AbilityObjectDataAsset> AbilityObjectDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_AbilityObjectAttributeSet> AbilityObjectAttributeSet;
};
