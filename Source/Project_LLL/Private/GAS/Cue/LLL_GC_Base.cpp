// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODBlueprintStatics.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	UObject* SourceObject = Parameters.EffectContext.GetSourceObject();
	if (IsValid(Parameters.EffectContext.GetSourceObject()))
	{
		ALLL_BaseCharacter* Character = CastChecked<ALLL_BaseCharacter>(MyTarget);
		Character->SetFModAudioComponent(UFMODBlueprintStatics::PlayEventAttached(FModEvent, MyTarget->GetRootComponent(),
			NAME_None, MyTarget->GetActorLocation(), EAttachLocation::KeepWorldPosition, true, true, true));
		UE_LOG(LogTemp, Log, TEXT("asdf"));
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
