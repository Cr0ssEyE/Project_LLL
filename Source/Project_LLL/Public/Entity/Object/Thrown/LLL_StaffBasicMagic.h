// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_StaffBasicMagicDataAsset.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "LLL_StaffBasicMagic.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_StaffBasicMagic : public ALLL_ThrownObject
{
	GENERATED_BODY()

public:
	ALLL_StaffBasicMagic();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void Activate() override;
	virtual void Deactivate() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> HitCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_StaffBasicMagicDataAsset> StaffBasicMagicDataAsset;
};
