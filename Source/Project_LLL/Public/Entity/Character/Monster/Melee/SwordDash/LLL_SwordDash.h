// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "Entity/Character/Monster/Melee/Base/LLL_MeleeMonster.h"
#include "LLL_SwordDash.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SwordDash : public ALLL_MeleeMonster, public ILLL_DashMonsterInterface
{
	GENERATED_BODY()

public:
	ALLL_SwordDash();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAttributeSet() override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	virtual float GetMaxDashDistance() const override;
	virtual float GetMinDashDistance() const override;
	virtual float GetDashSpeed() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;

	UPROPERTY(VisibleAnywhere)
	uint8 bIsDashing : 1;
};
