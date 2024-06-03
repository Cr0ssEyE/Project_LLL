// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/ObjectDebugWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/LLL_DebugGameInstance.h"

void UObjectDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ObjectGenerateCheckBox->OnCheckStateChanged.AddDynamic(this, &UObjectDebugWidget::ObjectGenerateCheckBoxEvent);
	ObjectActivateCheckBox->OnCheckStateChanged.AddDynamic(this, &UObjectDebugWidget::ObjectActivateCheckBoxEvent);
	ObjectDestroyCheckBox->OnCheckStateChanged.AddDynamic(this, &UObjectDebugWidget::ObjectDestroyCheckBoxEvent);
	ObjectHitCheckBox->OnCheckStateChanged.AddDynamic(this, &UObjectDebugWidget::ObjectHitCheckBoxEvent);
	ObjectCollisionCheckBox->OnCheckStateChanged.AddDynamic(this, &UObjectDebugWidget::ObjectCollisionCheckBoxEvent);
	ObjectTrapActivateButton->OnClicked.AddDynamic(this, &UObjectDebugWidget::ObjectTrapActivateButtonEvent);
}

void UObjectDebugWidget::ObjectGenerateCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetObjectGenerateDebug(ObjectGenerateCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectActivateCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetObjectActivateDebug(ObjectActivateCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectDestroyCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetObjectDestroyDebug(ObjectDestroyCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectHitCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetObjectHitCheckDebug(ObjectHitCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetObjectCollisionDebug(ObjectCollisionCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectTrapActivateButtonEvent()
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->BroadcastObjectTrapActivate();
}
