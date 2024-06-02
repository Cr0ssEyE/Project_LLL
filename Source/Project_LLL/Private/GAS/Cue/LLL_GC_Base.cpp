// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODEvent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Game/LLL_DebugGameInstance.h"
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
		if (const ULLL_DebugGameInstance* ProtoGameInstance = Cast<ULLL_DebugGameInstance>(GameInstance))
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

void ULLL_GC_Base::ReceiveSpawnResult(AActor* Target, const FGameplayCueNotify_SpawnResult& SpawnResult) const
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("블루프린트에서 함수 호출")));
	
	// FGameplayCueNotify_SpawnContext가 영문도 모른 채 코드에서 LNK2019 오류 뿜어대서 블루프린트로 우회 구현
	ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(Target);
	if (SpawnResult.FxSystemComponents.IsEmpty())
	{
		return;
	}

	for (auto SpawnComponent : SpawnResult.FxSystemComponents)
	{
		const UNiagaraComponent* Component = Cast<UNiagaraComponent>(SpawnComponent);
		if (!IsValid(Component))
		{
			continue;
		}

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("나이아가라 컴포넌트 생성 %s"), *Component->GetFName().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("나이아가라 시스템 이름 %s"), *Component->GetAsset()->GetFName().ToString()));
	}
}