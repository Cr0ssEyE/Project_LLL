// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Skill/Base/LLL_SkillObject.h"
#include "DataAsset/LLL_PlayerPhysicalProofDataAsset.h"
#include "LLL_Player_Deer_PhysicalProof.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_Player_Deer_PhysicalProof : public ALLL_SkillObject
{
	GENERATED_BODY()

public:
	ALLL_Player_Deer_PhysicalProof();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_PlayerPhysicalProofDataAsset> FeatherStormDataAsset;
};
