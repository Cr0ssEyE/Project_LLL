// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/LLL_AnimNotify_Niagara.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Interface/LLL_NiagaraInterface.h"

UFXSystemComponent* ULLL_AnimNotify_Niagara::SpawnEffect(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UFXSystemComponent* FXSystemComponent = nullptr;

	if (Template)
	{
		if (Template->IsLooping())
		{
			return FXSystemComponent;
		}

		if (Attached)
		{
			FXSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(Template, MeshComp, SocketName, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, true);
			if (ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(MeshComp->GetOwner()))
			{
				NiagaraInterface->AddNiagaraComponent(CastChecked<UNiagaraComponent>(FXSystemComponent));
			}
		}
		else
		{
			const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
			FXSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(MeshComp->GetWorld(), Template, SocketTransform.TransformPosition(LocationOffset), (SocketTransform.GetRotation() * RotationOffsetQuat).Rotator(), FVector::OneVector, true);
			if (ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(MeshComp->GetOwner()))
			{
				NiagaraInterface->AddNiagaraComponent(CastChecked<UNiagaraComponent>(FXSystemComponent));
			}
		}

		if (IsValid(FXSystemComponent))
		{
			FXSystemComponent->SetUsingAbsoluteScale(bAbsoluteScale);
			FXSystemComponent->SetRelativeScale3D_Direct(Scale);
		}
	}

	return FXSystemComponent;
}
