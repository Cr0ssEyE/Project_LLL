// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODAudioComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Object/Base/LLL_BaseObject.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const UObject* SourceObject = Parameters.EffectContext.GetSourceObject();
	if (IsValid(SourceObject))
	{
		const ALLL_BaseCharacter* Character = Cast<ALLL_BaseCharacter>(MyTarget);
		const ALLL_BaseObject* Object = Cast<ALLL_BaseObject>(MyTarget);

		UFMODAudioComponent* FModAudioComponent = nullptr;
		if (IsValid(Character))
		{
			FModAudioComponent = Character->GetFModAudioComponent();
		}
		else if (IsValid(Object))
		{
			FModAudioComponent = Object->GetFModAudioComponent();
		}

		if (IsValid(FModAudioComponent))
		{
			FModAudioComponent->Release();
			FModAudioComponent->Event = FModEvent;
			FModAudioComponent->Play();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("%s : FMod 컴포넌트가 없습니다"), *MyTarget->GetName())
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
