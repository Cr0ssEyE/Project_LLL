// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameInstance.h"
#include "System/Reward/LLL_RewardGimmick.h"
#include "UI/System/LLL_SelectRewardWidget.h"

void ALLL_AbilityRewardObject::SetInformation(const FRewardDataTable* Data, const uint32 Index)
{
	Super::SetInformation(Data);
	//매쉬 및 필수 정보 세팅
}

void ALLL_AbilityRewardObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);
	
	//AbilityRewardInteractionDelegate->Broadcast(AbilityCategory);
	
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(InteractedActor);
	
	ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();

	ALLL_RewardGimmick* RewardGimmick = Cast<ULLL_GameInstance>(GetGameInstance())->RewardGimmick;
	if (RewardGimmick->bIsTest)
	{
		SelectRewardWidget->Num = Num;
		RewardGimmick->SetRewardButtons();
	}
	
	SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
	SelectRewardWidget->SetIsEnabled(true);
	SelectRewardWidget->FocusToUI();
}

void ALLL_AbilityRewardObject::SetAbilityCategory(EAbilityCategory Category)
{
	AbilityCategory = Category;
}
