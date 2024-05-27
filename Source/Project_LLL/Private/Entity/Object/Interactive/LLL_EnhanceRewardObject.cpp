// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_EnhanceRewardObject.h"
#include "Game/ProtoGameInstance.h"

void ALLL_EnhanceRewardObject::SetInformation(FRewardDataTable* Data)
{
	Super::SetInformation(Data);
}

void ALLL_EnhanceRewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("능력 강화 보상 상호작용")));
		}
	}
#endif
}
