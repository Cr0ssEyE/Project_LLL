// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ThrownMagicDataAsset.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_Magic.generated.h"

class ULLL_MagicAttributeSet;
class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_Magic : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	ALLL_Magic();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> HitCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ThrownMagicDataAsset> MagicDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_MagicAttributeSet> MagicAttributeSet;
};
