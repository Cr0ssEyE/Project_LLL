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

protected:
	virtual void BeginPlay() override;
	virtual void InitAttributeSet() override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:
	virtual void Dash() override;

	FORCEINLINE virtual float GetDashDistance() const override { return DashDistance; }
	FORCEINLINE virtual float GetDashSpeed() const override { return DashSpeed; }

public:
	void Shockwave() const;
	void AttackInApnea();
	void SnapOtherMonster() const;
	void ThrowOtherMonster() const;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ManOfStrengthDataAsset> ManOfStrengthDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;

	UPROPERTY()
	uint8 bIsDashing : 1;
	
	UPROPERTY()
	float DashDistance;
	
	UPROPERTY()
	float DashSpeed;
};
