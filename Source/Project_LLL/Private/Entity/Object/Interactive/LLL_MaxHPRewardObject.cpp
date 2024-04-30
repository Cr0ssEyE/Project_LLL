// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_MaxHPRewardObject.h"

#include "Game/ProtoGameInstance.h"

void ALLL_MaxHPRewardObject::SetInformation(FTestRewardDataTable* Data)
{
	Super::SetInformation(Data);

	//매쉬 및 필수 정보 세팅
}

void ALLL_MaxHPRewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckObjectActivateDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("player 최대 체력 증가"));
			}
		}
#endif
}
