// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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
	FORCEINLINE void SetOffencePower(const float InOffencePower) { OffencePower = InOffencePower; }
	FORCEINLINE void SetKnockBackDirection(const FVector& InKnockBackDirection) { KnockBackDirection = InKnockBackDirection; }
	FORCEINLINE void SetKnockBackPower(const float InKnockBackPower) { KnockBackPower = InKnockBackPower; }

	virtual void BeginPlay() override;

protected:
	void DamageToOverlapActor(AActor* OtherActor);
	void KnockBackToOverlapActor(AActor* OtherActor);
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> OverlapCollisionSphere;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_AbilityObjectDataAsset> AbilityObjectDataAsset;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_AbilityObjectAttributeSet> AbilityObjectAttributeSet;

	const FAbilityDataTable* AbilityData;
	float AbilityLevel;
	
	float OffencePower;
	FVector KnockBackDirection;
	float KnockBackPower;
};
