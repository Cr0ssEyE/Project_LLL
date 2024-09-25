// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_ShockwavetDataAsset.h"
#include "Entity/Object/Ability/Base/LLL_AbilityObject.h"
#include "LLL_Shockwave.generated.h"

class ULLL_ShockwaveAttributeSet;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_Shockwave : public ALLL_AbilityObject
{
	GENERATED_BODY()

public:
	ALLL_Shockwave();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_ShockwavetDataAsset> ShockwaveDataAsset;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_ShockwaveAttributeSet> ShockwaveAttributeSet;

	uint8 bPlayerHit : 1;
};
