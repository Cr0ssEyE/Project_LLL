// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset/LLL_FeatherStormDataAsset.h"
#include "Entity/Object/Skill/Base/LLL_SkillObject.h"
#include "LLL_PlayerFeatherStorm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_PlayerFeatherStorm : public ALLL_SkillObject
{
	GENERATED_BODY()

public:
	ALLL_PlayerFeatherStorm();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_FeatherStormDataAsset> FeatherStormDataAsset;
};
