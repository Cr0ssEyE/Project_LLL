// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODAudioComponent.h"
#include "Entity/Object/Base/LLL_BaseObject.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const UObject* SourceObject = Parameters.EffectContext.GetSourceObject();
	if (IsValid(SourceObject))
	{
		const ILLL_FModInterface* FModInterface = Cast<ILLL_FModInterface>(MyTarget);

		UFMODAudioComponent* FModAudioComponent = FModInterface->GetFModAudioComponent();
		if (IsValid(FModAudioComponent))
		{
			FModAudioComponent->Release();
			FModAudioComponent->SetEvent(FModEvent);
			FModAudioComponent->SetPitch(MyTarget->CustomTimeDilation);
			FModAudioComponent->Play();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : FMod 컴포넌트가 없습니다"), *MyTarget->GetName())
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
