// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Base/LLL_BaseObject.h"
#include "LLL_SkillObject.generated.h"

class ULLL_SkillObjectAttributeSet;
class ULLL_SkillObjectDataAsset;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_SkillObject : public ALLL_BaseObject
{
	GENERATED_BODY()

public:
	ALLL_SkillObject();

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> SkillCollisionBox;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<const ULLL_SkillObjectDataAsset> SkillObjectDataAsset;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_SkillObjectAttributeSet> SkillObjectAttributeSet;
};
