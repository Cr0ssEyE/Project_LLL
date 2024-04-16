// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "LLL_SwordDash.generated.h"

class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SwordDash : public ALLL_MeleeMonster
{
	GENERATED_BODY()

public:
	ALLL_SwordDash();

	FORCEINLINE void SetDash(bool IsDashing) { bIsDashing = IsDashing; }
	FORCEINLINE bool IsDashing() const { return bIsDashing; }
	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	void Dash() const;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;

	uint8 bIsDashing : 1;
};
