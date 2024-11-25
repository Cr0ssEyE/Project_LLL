// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"

#include "NiagaraComponent.h"
#include "Constant/LLL_LevelNames.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameInstance.h"
#include "Game/LLL_RewardGimmickSubsystem.h"
#include "Kismet/GameplayStatics.h"
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
	NiagaraComponents[0]->OnSystemFinished.AddDynamic(this, &ALLL_AbilityRewardObject::ActiveUI);
	NiagaraComponents[0]->ActivateSystem();
	
}

void ALLL_AbilityRewardObject::SetAbilityCategory(EAbilityCategory Category)
{
	AbilityCategory = Category;
}


void ALLL_AbilityRewardObject::ActiveUI(UNiagaraComponent* PSystem)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();

	ULLL_RewardGimmickSubsystem* RewardGimmickSubsystem = Cast<ULLL_GameInstance>(GetGameInstance())->GetSubsystem<ULLL_RewardGimmickSubsystem>();
	if (RewardGimmickSubsystem->IsTest())
	{
		SelectRewardWidget->Num = Num;
		RewardGimmickSubsystem->SetRewardButtons();
	}
	if (GetWorld()->GetName() == LEVEL_SHOP)
	{
		RewardGimmickSubsystem->SetRewardButtons();
	}
	SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
	SelectRewardWidget->SetIsEnabled(true);
	SelectRewardWidget->FocusToUI();

	Destroy();
}
