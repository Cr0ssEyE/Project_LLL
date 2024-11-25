// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/LLL_AnimNotify_FMod.h"

#include "FMODBlueprintStatics.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Interface/LLL_FModInterface.h"
#include "Util/LLL_FModPlayHelper.h"

void ULLL_AnimNotify_FMod::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	static FFMODEventInstance Wrapper;
	
	AActor* OwnerActor = MeshComp->GetOwner();
	const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(OwnerActor);
	if (!FModInterface)
	{
		if (!bStop)
		{
			Wrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), Event, true);
		}
		else
		{
			UFMODBlueprintStatics::EventInstanceStop(Wrapper);
		}
		return;
	}

	FFModInfo FModInfo;
	FModInfo.FModEvent = Event;
	FModInfo.FModParameter = Parameter;
	FModInfo.SocketName = *AttachName;

	if (!bStop)
	{
		FLLL_FModPlayHelper::PlayFModEvent(OwnerActor, FModInfo);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		UGameInstance* GameInstance = OwnerActor->GetGameInstance();
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GameInstance))
		{
			if (DebugGameInstance->CheckSoundMessageDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *OwnerActor->GetName(), *Event->GetName()));
			}
		}
#endif
	}
	else
	{
		FLLL_FModPlayHelper::StopFModEvent(OwnerActor, FModInfo);
	}
}
