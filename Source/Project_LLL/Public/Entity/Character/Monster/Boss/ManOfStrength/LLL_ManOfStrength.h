// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ManOfStrengthDataAsset.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "LLL_ManOfStrength.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ManOfStrength : public ALLL_BossMonster, public ILLL_DashMonsterInterface
{
	GENERATED_BODY()

public:
	ALLL_ManOfStrength();
	
	FORCEINLINE virtual void SetDashing(bool IsDashing) override { bIsDashing = IsDashing; }
	
	FORCEINLINE virtual bool IsDashing() const override { return bIsDashing; }
	virtual float GetMaxDashDistance() const override { return DashDistance; }
	virtual float GetMinDashDistance() const override { return DashDistance; }
	virtual float GetDashSpeed() const override { return DashSpeed; }
	virtual float GetChargeTimer() const override { return ChargeTimer; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAttributeSet() override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	virtual void Charge() override;
	virtual void Dash() override {};

	void Shockwave() const;
	void DashPunch();
	void AttackInApnea();
	void SnapOtherMonster() const;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ManOfStrengthDataAsset> ManOfStrengthDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;

	UPROPERTY(VisibleDefaultsOnly)
	uint8 bIsDashing : 1;
	
	float DashDistance;
	float DashSpeed;
	float ChargeTimer;
};
