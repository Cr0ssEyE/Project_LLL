// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_SwordDashDataAsset.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"
#include "Interface/LLL_ChargeMonsterInterface.h"
#include "Interface/LLL_DashMonsterInterface.h"
#include "LLL_SwordDash.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SwordDash : public ALLL_MeleeMonster, public ILLL_DashMonsterInterface, public ILLL_ChargeMonsterInterface
{
	GENERATED_BODY()

public:
	ALLL_SwordDash();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void DashDamageRangeBoxInit() const;

public:
	virtual void Dash() const override;
	virtual void Charge() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SwordDashDataAsset> SwordDashDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> DashDamageRangeBox;
};
