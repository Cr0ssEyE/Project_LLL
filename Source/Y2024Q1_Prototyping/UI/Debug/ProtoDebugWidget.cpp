// Fill out your copyright notice in the Description page of Project Settings.


#include "Y2024Q1_Prototyping/UI/Debug/ProtoDebugWidget.h"

void UProtoDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnablePlayerDebugWidget);
	MonsterDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableMonsterDebugWidget);
	ObjectDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableObjectDebugWidget);
	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UProtoDebugWidget::EnablePlayerDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetTypes::Player));
}

void UProtoDebugWidget::EnableMonsterDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetTypes::Monster));
}

void UProtoDebugWidget::EnableObjectDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetTypes::Object));
}

void UProtoDebugWidget::ToggleWidget()
{
	SetIsEnabled(!GetIsEnabled());
	GetIsEnabled() == 0 ? SetVisibility(ESlateVisibility::Hidden) : SetVisibility(ESlateVisibility::Visible);
}
