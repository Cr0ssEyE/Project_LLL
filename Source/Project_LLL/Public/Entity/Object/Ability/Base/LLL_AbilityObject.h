// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "LLL_AbilityObject.generated.h"

class USphereComponent;
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

	FORCEINLINE void SetAbilityInfo(const FAbilityDataTable* InAbilityData, float InAbilityLevel) { AbilityData = InAbilityData; AbilityLevel = InAbilityLevel; }

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UFUNCTION()
	void DamageToOverlapActor(AActor* Actor);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> OverlapCollisionSphere;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_AbilityObjectDataAsset> AbilityObjectDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_AbilityObjectAttributeSet> AbilityObjectAttributeSet;

	const FAbilityDataTable* AbilityData;
	float AbilityLevel;
};
