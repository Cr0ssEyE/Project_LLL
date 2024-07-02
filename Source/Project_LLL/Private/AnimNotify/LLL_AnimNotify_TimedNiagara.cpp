// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/LLL_AnimNotify_TimedNiagara.h"

#include "Particles/ParticleSystemComponent.h"

ULLL_AnimNotify_TimedNiagara::ULLL_AnimNotify_TimedNiagara() :
	Scale(FVector::One()),
	bAbsoluteScale(false)
{
}

UFXSystemComponent* ULLL_AnimNotify_TimedNiagara::SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
	UFXSystemComponent* Component = Super::SpawnEffect(MeshComp, Animation);

	if (Component != nullptr)
	{
		Component->SetUsingAbsoluteScale(bAbsoluteScale);
		Component->SetRelativeScale3D_Direct(Scale);
	}
	
	return Component;
}
