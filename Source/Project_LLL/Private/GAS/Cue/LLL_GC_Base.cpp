// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Cue/LLL_GC_Base.h"

#include "FMODEvent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_FModPlayHelper.h"

bool ULLL_GC_Base::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	for (const auto FModInfo : FModInfos)
	{
		FLLL_FModPlayHelper::PlayFModEvent(MyTarget, FModInfo);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		UGameInstance* GameInstance = MyTarget->GetGameInstance();
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GameInstance))
		{
			if (DebugGameInstance->CheckSoundMessageDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s 액터가 %s 재생"), *MyTarget->GetName(), *FModInfo.FModEvent->GetName()));
			}
		}
#endif
	}

	UE_LOG(LogTemp, Log, TEXT("%s 큐 발동"), *GetName())
	
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}

void ULLL_GC_Base::ReceiveSpawnResult(AActor* Target, const FGameplayCueNotify_SpawnResult& SpawnResult) const
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	// FGameplayCueNotify_SpawnContext가 영문도 모른 채 코드에서 LNK2019 오류 뿜어대서 블루프린트로 우회 구현
	ILLL_NiagaraInterface* NiagaraInterface = Cast<ILLL_NiagaraInterface>(Target);
	if (!NiagaraInterface)
	{
		return;
	}
	
	if (SpawnResult.FxSystemComponents.IsEmpty())
	{
		return;
	}

	for (auto SpawnComponent : SpawnResult.FxSystemComponents)
	{
		UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(SpawnComponent);
		if (!IsValid(NiagaraComponent))
		{
			continue;
		}

		NiagaraInterface->AddNiagaraComponent(NiagaraComponent);

		const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		FVector FacingDirection = NiagaraComponent->GetComponentRotation().Vector();
		switch (EffectFacingTarget)
		{
		case EEffectFacingSetting::LookPlayer:
			FacingDirection = (NiagaraComponent->GetComponentLocation() - PlayerCharacter->GetMesh()->GetComponentLocation()).GetSafeNormal2D();
			FacingDirection.Z = 0.f;

			NiagaraComponent->SetWorldRotation(FacingDirection.Rotation() + PlayerCharacter->GetMesh()->GetRelativeRotation());
			break;
		case EEffectFacingSetting::PlayerForward:
			FacingDirection = -PlayerCharacter->GetMesh()->GetForwardVector();
			FacingDirection.Z = 0.f;
			NiagaraComponent->SetWorldRotation(FacingDirection.Rotation());
			break;
		case EEffectFacingSetting::LookCamera:
			FacingDirection = -PlayerCharacter->GetPlayerCamera()->GetForwardVector();
			NiagaraComponent->SetWorldRotation(FacingDirection.Rotation());
			break;
		default:
			break;
		}
	}
}