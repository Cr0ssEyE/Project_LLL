// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerUIManager.h"

#include "Components/WidgetComponent.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Entity/Character/Player/LLL_InteractionWidget.h"
#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"
#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "UI/Entity/Character/Player/LLL_PlayerComboWidget.h"
#include "UI/Entity/Character/Player/LLL_PlayerStatusWidget.h"
#include "UI/Entity/Character/Player/LLL_SkillWidget.h"
#include "UI/System/LLL_GamePauseWidget.h"
#include "UI/System/LLL_SelectRewardWidget.h"

ULLL_PlayerUIManager::ULLL_PlayerUIManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULLL_PlayerUIManager::BeginPlay()
{
	Super::BeginPlay();

	const ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(GetOwner());
	const ULLL_BaseCharacterDataAsset* CharacterDataAsset = PlayerCharacter->GetCharacterDataAsset();
	const ULLL_PlayerBaseDataAsset* PlayerBaseDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
	
	CharacterStatusWidgetClass = CharacterDataAsset->StatusWidgetClass;
	GamePauseWidgetClass = PlayerBaseDataAsset->GamePauseWidgetClass;
	InventoryWidgetClass = PlayerBaseDataAsset->InventoryWidgetClass;
	InteractionWidgetClass = PlayerBaseDataAsset->InteractionWidgetClass;
	SelectRewardWidgetClass = PlayerBaseDataAsset->SelectRewardWidgetClass;
	ChaseActionWidgetClass = PlayerBaseDataAsset->ChaseActionWidgetClass;
	ComboWidgetClass = PlayerBaseDataAsset->ComboWidgetClass;
	MainEruriaInfoWidgetClass = PlayerBaseDataAsset->MainEruriaInfoWidgetClass;

	if(IsValid(SelectRewardWidgetClass))
	{
		SelectRewardWidget = CastChecked<ULLL_SelectRewardWidget>(CreateWidget(GetWorld(), SelectRewardWidgetClass));
		SelectRewardWidget->AddToViewport(1);
		SelectRewardWidget->SetVisibility(ESlateVisibility::Hidden);
		SelectRewardWidget->SetIsEnabled(false);
	}
	
	if(IsValid(CharacterStatusWidgetClass))
	{
		CharacterStatusWidget = CastChecked<ULLL_CharacterStatusWidget>(CreateWidget(GetWorld(), CharacterStatusWidgetClass));
		CharacterStatusWidget->AddToViewport(3);
	}

	if (IsValid(MainEruriaInfoWidgetClass))
	{
		MainEruriaInfoWidget = CastChecked<ULLL_MainEruriaInfoWidget>(CreateWidget(GetWorld(), MainEruriaInfoWidgetClass));
		MainEruriaInfoWidget->AddToViewport(3);
	}
	
	if(IsValid(InventoryWidgetClass))
	{
		InventoryWidget = CastChecked<ULLL_InventoryWidget>(CreateWidget(GetWorld(), InventoryWidgetClass));
		InventoryWidget->AddToViewport(2);
		InventoryWidget->SetIsEnabled(false);
	}

	if(IsValid(InteractionWidgetClass))
	{
		InteractionWidget = CastChecked<ULLL_InteractionWidget>(CreateWidget(GetWorld(), InteractionWidgetClass));
		InteractionWidget->AddToViewport();
		InteractionWidget->SetIsEnabled(false);
	}
	
	if(IsValid(ComboWidgetClass))
	{
		ComboWidget = CastChecked<ULLL_PlayerComboWidget>(CreateWidget(GetWorld(), ComboWidgetClass));
		ComboWidget->AddToViewport();
		ComboWidget->SetComboText(0);
	}

	if(IsValid(GamePauseWidgetClass))
	{
		GamePauseWidget = CastChecked<ULLL_GamePauseWidget>(CreateWidget(GetWorld(), GamePauseWidgetClass));
		GamePauseWidget->AddToViewport(99);
		GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
		GamePauseWidget->SetIsEnabled(false);
	}

	if(IsValid(ChaseActionWidgetClass))
	{
		ChaseActionWidget = CastChecked<ULLL_PlayerChaseActionWidget>(CreateWidget(GetWorld(), ChaseActionWidgetClass));
	}
}

void ULLL_PlayerUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwner());
	ChaseActionWidget->UpdateWidgetView(Player->GetAbilitySystemComponent());
}

void ULLL_PlayerUIManager::TogglePauseWidget(bool IsDead) const
{
	if(GamePauseWidget->GetIsEnabled())
	{
		bool IsUniquePopup = SelectRewardWidget->GetIsEnabled();
		GamePauseWidget->RestorePauseState(!IsUniquePopup);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
	}
	else
	{
		if(IsDead)
		{
			GamePauseWidget->SetupDeadStateLayout();
		}
		GamePauseWidget->SetupPauseState();
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

void ULLL_PlayerUIManager::UpdateInteractionWidget(const ALLL_InteractiveObject* CurrentObject, int Num) const
{
	// InteractionWidget->RenderNextInteractionPanel(static_cast<bool>(Num));
	InteractionWidget->SetInfoText(CurrentObject->GetActorNameOrLabel());
}

void ULLL_PlayerUIManager::SetAllWidgetVisibility(const bool Visible) const
{
	if(!Visible)
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		CharacterStatusWidget->SetVisibility(ESlateVisibility::Hidden);
		InteractionWidget->SetVisibility(ESlateVisibility::Hidden);
		MainEruriaInfoWidget->SetVisibility(ESlateVisibility::Hidden);
		ComboWidget->SetVisibility(ESlateVisibility::Hidden);
		ChaseActionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		CharacterStatusWidget->SetVisibility(ESlateVisibility::Visible);
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		MainEruriaInfoWidget->SetVisibility(ESlateVisibility::Visible);
		ComboWidget->SetVisibility(ESlateVisibility::Visible);
		ChaseActionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULLL_PlayerUIManager::UpdateWidget()
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwner());
	const ULLL_PlayerStatusWidget* PlayerStatusWidget = CastChecked<ULLL_PlayerStatusWidget>(CharacterStatusWidget);
	PlayerStatusWidget->UpdateWidgetView(Player->GetAbilitySystemComponent());
	Super::UpdateWidget();
}

