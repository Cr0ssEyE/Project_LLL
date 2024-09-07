// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_EnhanceRewardObject.h"
#include "Game/LLL_DebugGameInstance.h"

void ALLL_EnhanceRewardObject::SetInformation(const FRewardDataTable* Data)
{
	Super::SetInformation(Data);
}

void ALLL_EnhanceRewardObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("능력 강화 보상 상호작용")));
		}
	}
#endif
}
