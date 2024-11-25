// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FMODAudioComponent.h"
#include "Util/LLL_FModInfoStruct.h"
#include "Entity/Object/Base/LLL_BaseObject.h"

class PROJECT_LLL_API FLLL_FModPlayHelper
{
public:
	static void PlayFModEvent(AActor* SourceActor, const FFModInfo& FModInfo)
	{
		if (ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(SourceActor))
		{
			UFMODAudioComponent* FModAudioComponent = FModInterface->GetFModAudioComponent();
			FModAudioComponent->Release();
			FModAudioComponent->AttachToComponent(SourceActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, FModInfo.SocketName);
			FModAudioComponent->SetEvent(FModInfo.FModEvent);
			FModAudioComponent->SetPitch(SourceActor->CustomTimeDilation);
			FModAudioComponent->Play();
			FModInterface->SetFModParameter(FModInfo.FModParameter);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : FMod 컴포넌트가 없습니다"), *SourceActor->GetName())
		}
	}
	
	static void StopFModEvent(AActor* SourceActor, const FFModInfo& FModInfo)
	{
		if (ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(SourceActor))
		{
			UFMODAudioComponent* FModAudioComponent = FModInterface->GetFModAudioComponent();
			FModAudioComponent->SetEvent(FModInfo.FModEvent);
			FModAudioComponent->Stop();
			FModAudioComponent->Release();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : FMod 컴포넌트가 없습니다"), *SourceActor->GetName())
		}
	}
};
