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

	virtual void Tick(float DeltaSeconds) override;
	
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }
	FORCEINLINE virtual void SetPitch(float InPitch) override { Pitch = InPitch; }

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;

	UPROPERTY(EditAnywhere)
	float Pitch;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentPitch;
};
