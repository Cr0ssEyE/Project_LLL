// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "Entity/Object/Base/LLL_BaseObject.h"

class PROJECT_LLL_API FLLL_FModPlayHelper
{
public:
	static void PlayFModEvent(const AActor* SourceActor, UFMODEvent* FModEvent)
	{
		if (const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(SourceActor))
		{
			UFMODAudioComponent* FModAudioComponent = FModInterface->GetFModAudioComponent();
			FModAudioComponent->Release();
			FModAudioComponent->SetEvent(FModEvent);
			FModAudioComponent->SetPitch(SourceActor->CustomTimeDilation);
			FModAudioComponent->Play();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : GAS 컴포넌트가 없습니다"), *SourceActor->GetName())
		}
	}
};
