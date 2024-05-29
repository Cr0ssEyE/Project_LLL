// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/ProtoDebugWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Enumeration/LLL_UserInterfaceEnumHelper.h"
#include "Game/ProtoGameInstance.h"

void UProtoDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnablePlayerDebugWidget);
	MonsterDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableMonsterDebugWidget);
	ObjectDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableObjectDebugWidget);
	SoundDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::ToggleSoundMessage);
	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UProtoDebugWidget::EnablePlayerDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Player));
}

void UProtoDebugWidget::EnableMonsterDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Monster));
}

void UProtoDebugWidget::EnableObjectDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Object));
}

void UProtoDebugWidget::ToggleWidget()
{
	SetIsEnabled(!GetIsEnabled());
	GetIsEnabled() == 0 ? SetVisibility(ESlateVisibility::Hidden) : SetVisibility(ESlateVisibility::Visible);
}

void UProtoDebugWidget::ToggleSoundMessage()
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->ToggleSoundMessageDebug();
}
