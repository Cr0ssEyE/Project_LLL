// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODAudioComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	const UObject* SourceObject = Parameters.EffectContext.GetSourceObject();
	if (IsValid(SourceObject))
	{
		const ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(MyTarget);
		UFMODAudioComponent* FModAudioComponent = Character->GetFModAudioComponent();
		FModAudioComponent->Event = FModEvent;
		FModAudioComponent->Play();
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
