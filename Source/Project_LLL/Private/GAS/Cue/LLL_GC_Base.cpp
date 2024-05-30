// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODEvent.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLL_FModPlayHelper.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	FLLL_FModPlayHelper::PlayFModEvent(MyTarget, FModEvent, FModParameter, SocketName);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	UGameInstance* GameInstance = MyTarget->GetGameInstance();
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GameInstance))
	{
		if (ProtoGameInstance->CheckSoundMessageDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *MyTarget->GetName(), *FModEvent->GetName()));
		}
	}
#endif
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
