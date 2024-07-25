// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "LLL_DungeonEnteringObject.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_DungeonEnteringObject : public ALLL_InteractiveObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_DungeonEnteringObject();

	virtual void InteractiveEvent(AActor* InteractedActor = nullptr) override;
	
protected:
	
};
