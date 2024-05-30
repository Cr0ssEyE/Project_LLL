// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/LLL_AnimNotify_FMod.h"

#include "FMODBlueprintStatics.h"
#include "Game/ProtoGameInstance.h"
#include "Interface/LLL_FModInterface.h"
#include "Util/LLL_FModPlayHelper.h"

void ULLL_AnimNotify_FMod::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(OwnerActor);
	if (!FModInterface)
	{
		UFMODBlueprintStatics::PlayEvent2D(GetWorld(), Event, true);
		return;
	}

	FFModInfo FModInfo;
	FModInfo.FModEvent = Event;
	FModInfo.FModParameter = Parameter;
	FModInfo.SocketName = *AttachName;

	FLLL_FModPlayHelper::PlayFModEvent(OwnerActor, FModInfo);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	UGameInstance* GameInstance = OwnerActor->GetGameInstance();
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GameInstance))
	{
		if (ProtoGameInstance->CheckSoundMessageDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *OwnerActor->GetName(), *Event->GetName()));
		}
	}
#endif
}
