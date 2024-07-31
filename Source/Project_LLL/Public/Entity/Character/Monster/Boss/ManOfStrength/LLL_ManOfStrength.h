// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ManOfStrengthDataAsset.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "LLL_ManOfStrength.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_ManOfStrength : public ALLL_BossMonster
{
	GENERATED_BODY()

public:
	ALLL_ManOfStrength();

protected:
	virtual void BeginPlay() override;

public:
	void Shockwave() const;
	void AttackInApnea() const;
	void SnapOtherMonster() const;
	
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ManOfStrengthDataAsset> ManOfStrengthDataAsset;
};
