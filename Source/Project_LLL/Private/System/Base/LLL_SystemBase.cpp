// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base/LLL_SystemBase.h"

#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"

ALLL_SystemBase::ALLL_SystemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FModAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FModAudioComponent"));
	FModAudioComponent->SetupAttachment(RootComponent);
}

void ALLL_SystemBase::AddNiagaraComponent(UNiagaraComponent* InNiagaraComponent)
{
	NiagaraComponents.Remove(nullptr);
	NiagaraComponents.Emplace(InNiagaraComponent);
}
