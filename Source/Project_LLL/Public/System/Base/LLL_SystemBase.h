// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LLL_SystemBase.generated.h"

class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_SystemBase : public AActor
{
	GENERATED_BODY()

public:
	ALLL_SystemBase();
	
	FORCEINLINE UFMODAudioComponent* GetFModAudioComponent() const { return FModAudioComponent; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;
};
