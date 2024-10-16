// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GoldRewardObject.h"

#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void ALLL_GoldRewardObject::SetInformation(const FRewardDataTable* Data)
{
	Super::SetInformation(Data);

	//매쉬 및 필수 정보 세팅
	RewardValue = RewardData->Value;
}

void ALLL_GoldRewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	ULLL_PlayerGoldComponent* PlayerGoldComponent = Player->GetGoldComponent();
	PlayerGoldComponent->IncreaseMoney(RewardValue);
}
