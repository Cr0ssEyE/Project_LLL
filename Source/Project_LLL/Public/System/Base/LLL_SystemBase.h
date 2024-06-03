// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_FModInterface.h"
#include "Interface/LLL_NiagaraInterface.h"
#include "LLL_SystemBase.generated.h"

class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_SystemBase : public AActor, public ILLL_FModInterface, public ILLL_NiagaraInterface
{
	GENERATED_BODY()

public:
	ALLL_SystemBase();
	
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }

protected:
	virtual void SetFModParameter(EFModParameter FModParameter) override {}
	virtual void SetNiagaraComponent(UNiagaraComponent* InNiagaraComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraComponent>> NiagaraComponents;
};
