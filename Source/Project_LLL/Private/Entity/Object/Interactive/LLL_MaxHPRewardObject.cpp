// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_MaxHPRewardObject.h"

#include "GameplayEffectTypes.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Game/LLL_GameProgressManageSubSystem.h"

void ALLL_MaxHPRewardObject::SetInformation(const FRewardDataTable* Data, const uint32 Index)
{
	Super::SetInformation(Data);

	//매쉬 및 필수 정보 세팅
}

void ALLL_MaxHPRewardObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(InteractedActor);
	FGameplayEffectContextHandle EffectContextHandle = Player->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	EffectContextHandle.AddInstigator(this, this);
	const FGameplayEffectSpecHandle EffectSpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(RewardObjectDataAsset->MaxHPEffect, 1.0, EffectContextHandle);

	if(EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
		GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()->PlayerPlayProgressData.AcquiredGoldAppleCount++;
	}
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(DebugGameInstance->CheckObjectActivateDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("player 최대 체력 증가"));
			}
		}
#endif
	Destroy();
}
