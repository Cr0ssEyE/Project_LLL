// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "UI/System/LLL_SelectRewardWidget.h"

void ALLL_AbilityRewardObject::SetInformation(FTestRewardDataTable* Data)
{
	Super::SetInformation(Data);
	//매쉬 및 필수 정보 세팅
}

void ALLL_AbilityRewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());

	ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();
	
	SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
	SelectRewardWidget->SetIsEnabled(true);
}

void ALLL_AbilityRewardObject::SetAbilityData(TArray<FTestAbilityDataTable> AbilityData)
{
	RewardAbilityData = AbilityData;
}
