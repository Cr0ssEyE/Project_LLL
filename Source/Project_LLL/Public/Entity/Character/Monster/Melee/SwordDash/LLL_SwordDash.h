// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"
#include "LLL_SwordDash.generated.h"

class UBoxComponent;
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
	FORCEINLINE void SetCharge(bool IsCharging) { bIsCharging = IsCharging; }
	
	FORCEINLINE bool IsDashing() const { return bIsDashing; }
	FORCEINLINE bool IsCharging() const { return bIsCharging; }
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void DashDamageRangeBoxInit() const;

public:
	void Dash() const;
	void Charge() const;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;

	uint8 bIsDashing : 1;
	uint8 bIsCharging : 1;
};
