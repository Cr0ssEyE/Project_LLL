// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_FModInterface.h"
#include "LLL_SystemBase.generated.h"

class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_SystemBase : public AActor, public ILLL_FModInterface
{
	GENERATED_BODY()

public:
	ALLL_SystemBase();
	
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }

protected:
	virtual void SetFModParameter(EFModParameter FModParameter) override {}

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;
};
