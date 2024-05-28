// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Animation/LLL_AnimNotify_FMod.h"

#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "Game/ProtoGameInstance.h"
#include "Interface/LLL_FModInterface.h"
#include "Util/LLL_FModPlayHelper.h"

void ULLL_AnimNotify_FMod::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	FName ParameterName;
	if (!FModParameterRowHandle.IsNull())
	{
		ParameterName = FModParameterRowHandle.GetRow<FFModParameterDataTable>(TEXT("Not Found FModParameter Row"))->Name;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(OwnerActor);
	if (!FModInterface)
	{
		const FFMODEventInstance DebugWrapper = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), FModEvent, true);
		if (!FModParameterRowHandle.IsNull())
		{
			UFMODBlueprintStatics::EventInstanceSetParameter(DebugWrapper, ParameterName, FModParameterValue);
		}
		return;
	}
	
	FLLL_FModPlayHelper::PlayFModEvent(OwnerActor, FModEvent);

	UFMODAudioComponent* FModAudioComponent = FModInterface->GetFModAudioComponent();
	if (!FModParameterRowHandle.IsNull())
	{
		FModAudioComponent->SetParameter(ParameterName, FModParameterValue);
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	UGameInstance* GameInstance = OwnerActor->GetGameInstance();
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GameInstance))
	{
		if (ProtoGameInstance->CheckSoundMessageDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *OwnerActor->GetName(), *FModEvent->GetName()));
		}
	}
#endif
}
