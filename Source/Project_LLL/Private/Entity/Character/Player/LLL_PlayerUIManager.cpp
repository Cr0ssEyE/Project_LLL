// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerUIManager.h"

#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Player/LLL_InteractionWidget.h"
#include "UI/Player/LLL_InventoryWidget.h"
#include "UI/Player/LLL_PlayerStatusWidget.h"
#include "UI/System/LLL_GamePauseWidget.h"

// Sets default values for this component's properties
ULLL_PlayerUIManager::ULLL_PlayerUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULLL_PlayerUIManager::BeginPlay()
{
	Super::BeginPlay();

	const ULLL_BaseCharacterDataAsset* CharacterDataAsset = CastChecked<ALLL_PlayerBase>(GetOwner())->GetCharacterDataAsset();
	const ULLL_PlayerBaseDataAsset* PlayerBaseDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
	
	CharacterStatusWidgetClass = CharacterDataAsset->StatusWidgetClass;
	GamePauseWidgetClass = PlayerBaseDataAsset->GamePauseWidgetClass;
	InventoryWidgetClass = PlayerBaseDataAsset->InventoryWidgetClass;
	InteractionWidgetClass = PlayerBaseDataAsset->InteractionWidgetClass;

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
}


// Called every frame
void ULLL_PlayerUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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
	}
	else
	{
		GamePauseWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		CharacterStatusWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

