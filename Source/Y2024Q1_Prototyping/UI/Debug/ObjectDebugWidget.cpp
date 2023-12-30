// Fill out your copyright notice in the Description page of Project Settings.


#include "Y2024Q1_Prototyping/UI/Debug/ObjectDebugWidget.h"

#include "Y2024Q1_Prototyping/Game/ProtoGameInstance.h"

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
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetObjectGenerateDebug(ObjectGenerateCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectActivateCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetObjectActivateDebug(ObjectActivateCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectDestroyCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetObjectDestroyDebug(ObjectDestroyCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectHitCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetObjectHitCheckDebug(ObjectHitCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetObjectCollisionDebug(ObjectCollisionCheckBox->IsChecked());
}

void UObjectDebugWidget::ObjectTrapActivateButtonEvent()
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->ObjectTrapActivateDelegate.Broadcast();
}
