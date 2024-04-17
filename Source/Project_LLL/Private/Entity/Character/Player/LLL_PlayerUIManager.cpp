// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerUIManager.h"

#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Player/LLL_InteractionWidget.h"
#include "UI/Player/LLL_InventoryWidget.h"
#include "UI/Player/LLL_PlayerStatusWidget.h"
#include "UI/Player/LLL_SkillWidget.h"
#include "UI/System/LLL_GamePauseWidget.h"

ULLL_PlayerUIManager::ULLL_PlayerUIManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULLL_PlayerUIManager::BeginPlay()
{
	Super::BeginPlay();

	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetOwner());
	const ULLL_BaseCharacterDataAsset* CharacterDataAsset = PlayerCharacter->GetCharacterDataAsset();
	const ULLL_PlayerBaseDataAsset* PlayerBaseDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
	
	CharacterStatusWidgetClass = CharacterDataAsset->StatusWidgetClass;
	GamePauseWidgetClass = PlayerBaseDataAsset->GamePauseWidgetClass;
	InventoryWidgetClass = PlayerBaseDataAsset->InventoryWidgetClass;
	InteractionWidgetClass = PlayerBaseDataAsset->InteractionWidgetClass;
	SkillGaugeWidgetClass = PlayerBaseDataAsset->SkillGaugeWidgetClass;
	
	if(IsValid(CharacterStatusWidgetClass))
	{
		CharacterStatusWidget = CastChecked<ULLL_CharacterStatusWidget>(CreateWidget(GetWorld(), CharacterStatusWidgetClass));
		CharacterStatusWidget->AddToViewport();
	}
	
	if(IsValid(GamePauseWidgetClass))
	{
		GamePauseWidget = CastChecked<ULLL_GamePauseWidget>(CreateWidget(GetWorld(), GamePauseWidgetClass));
		GamePauseWidget->AddToViewport();
		GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
		GamePauseWidget->SetIsEnabled(false);
	}

	if(IsValid(InventoryWidgetClass))
	{
		InventoryWidget = CastChecked<ULLL_InventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
		InventoryWidget->AddToViewport();
		InventoryWidget->SetIsEnabled(false);
	}

	if(IsValid(InteractionWidgetClass))
	{
		InteractionWidget = CastChecked<ULLL_InteractionWidget>(CreateWidget(GetWorld(), InteractionWidgetClass));
		InteractionWidget->AddToViewport();
		InteractionWidget->SetIsEnabled(false);
	}

	if(IsValid(SkillGaugeWidgetClass))
	{
		SkillGaugeWidget = CastChecked<ULLL_SkillWidget>(CreateWidget(GetWorld(), SkillGaugeWidgetClass));
		SkillGaugeWidget->AddToViewport();
	}
}

void ULLL_PlayerUIManager::TogglePauseWidget(bool IsDead) const
{
	if(GamePauseWidget->GetIsEnabled())
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
		GamePauseWidget->SetIsEnabled(false);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}
	else
	{
		if(IsDead)
		{
			GamePauseWidget->SetupDeadStateLayout();
		}
		GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
		GamePauseWidget->SetIsEnabled(true);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), SMALL_NUMBER);
	}
}

void ULLL_PlayerUIManager::ToggleInventoryWidget() const
{
	//TODO: 애니메이션 처리 여부 생각하기
	if(!InventoryWidget->IsPlayingAnimation())
	{
		if(InventoryWidget->GetIsEnabled())
		{
			InventoryWidget->PlayCloseAnimation();
			InventoryWidget->SetIsEnabled(false);
		}
		else
		{
			InventoryWidget->PlayOpenAnimation();
			InventoryWidget->SetIsEnabled(true);
		}
	}
}

void ULLL_PlayerUIManager::EnableInteractionWidget() const
{
	if(!InteractionWidget->GetIsEnabled())
	{
		InteractionWidget->PlayVisibleAnimation();
		InteractionWidget->SetIsEnabled(true);
	}
}

void ULLL_PlayerUIManager::DisableInteractionWidget() const
{
	InteractionWidget->PlayHideAnimation();
	InteractionWidget->SetIsEnabled(false);
}

void ULLL_PlayerUIManager::UpdateInteractionWidget(ALLL_InteractiveObject* CurrentObject, int Num) const
{
	InteractionWidget->RenderNextInteractionPanel(static_cast<bool>(Num));
	InteractionWidget->SetInfoText(CurrentObject->GetActorNameOrLabel());
}

void ULLL_PlayerUIManager::SetAllWidgetVisibility(const bool Visible)
{
	if(Visible)
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		CharacterStatusWidget->SetVisibility(ESlateVisibility::Hidden);
		SkillGaugeWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		CharacterStatusWidget->SetVisibility(ESlateVisibility::Visible);
		SkillGaugeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULLL_PlayerUIManager::UpdateWidget()
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwner());
	const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(Player->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));

	SkillGaugeWidget->UpdateWidgetView(PlayerAttributeSet);
	
	Super::UpdateWidget();
}

