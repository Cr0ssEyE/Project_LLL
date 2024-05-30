// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODEvent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/Character.h"
#include "Util/LLL_FModPlayHelper.h"
#include "Util/LLL_NiagaraInfoStruct.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	for (const auto FModInfo : FModInfos)
	{
		FLLL_FModPlayHelper::PlayFModEvent(MyTarget, FModInfo);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		UGameInstance* GameInstance = MyTarget->GetGameInstance();
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GameInstance))
		{
			if (ProtoGameInstance->CheckSoundMessageDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *MyTarget->GetName(), *FModInfo.FModEvent->GetName()));
			}
		}
#endif
	}

	for (const auto NiagaraInfo : NiagaraInfos)
	{
		if (IsValid(NiagaraInfo.NiagaraSystem))
		{
			if (NiagaraInfo.NiagaraSystem->IsLooping())
			{
				continue;
			}

			UNiagaraComponent* NiagaraComponent;
			
			const ACharacter* Character = Cast<ACharacter>(MyTarget);
			USceneComponent* AttachToComponent = IsValid(Character) ? Character->GetMesh() : MyTarget->GetRootComponent();
			
			if (NiagaraInfo.AttacheToTarget)
			{
				NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraInfo.NiagaraSystem, AttachToComponent, NiagaraInfo.SocketName, NiagaraInfo.Location, NiagaraInfo.Rotation, EAttachLocation::KeepRelativeOffset, true);

				if (ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(MyTarget))
				{
					NiagaraInterface->SetNiagaraComponent(NiagaraComponent);
				}
			}
			else
			{
				const USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();
				const FTransform SocketTransform = SkeletalMeshComponent->GetSocketTransform(NiagaraInfo.SocketName);
				NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraInfo.NiagaraSystem, SocketTransform.TransformPosition(NiagaraInfo.Location), (SocketTransform.GetRotation() * FQuat(NiagaraInfo.Rotation)).Rotator(), FVector::OneVector, true);

				if (ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(MyTarget))
				{
					NiagaraInterface->SetNiagaraComponent(NiagaraComponent);
				}
			}
			
			if (IsValid(NiagaraComponent))
			{
				NiagaraComponent->SetUsingAbsoluteScale(NiagaraInfo.UseAbsoluteScale);
				NiagaraComponent->SetRelativeScale3D_Direct(NiagaraInfo.Scale);
			}
		}
	}
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
