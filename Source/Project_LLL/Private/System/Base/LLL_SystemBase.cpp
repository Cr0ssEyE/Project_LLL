// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Base/LLL_SystemBase.h"

#include "FMODAudioComponent.h"

ALLL_SystemBase::ALLL_SystemBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	FModAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FModAudioComponent"));
	FModAudioComponent->SetupAttachment(RootComponent);

	Pitch = 1.0f;
	CurrentPitch = 1.0f;
}

void ALLL_SystemBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentPitch != Pitch)
	{
		CurrentPitch = FMath::FInterpTo(CurrentPitch, Pitch, DeltaSeconds, 1.0f);
		
		FModAudioComponent->SetPitch(CurrentPitch);
	}
}
