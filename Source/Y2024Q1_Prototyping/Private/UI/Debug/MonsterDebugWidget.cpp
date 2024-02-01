// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/MonsterDebugWidget.h"
#include "Components/CheckBox.h"
#include "Game/ProtoGameInstance.h"

void UMonsterDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MonsterAIDisableCheckBox->SetCheckedState(ECheckBoxState::Checked);
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterToggleAIDebug(MonsterAIDisableCheckBox->IsChecked());
	
	MonsterAIDisableCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterToggleAICheckBoxEvent);
	MonsterHitCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterHitCheckCheckBoxEvent);
	MonsterAttackCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterAttackCheckBoxEvent);
	MonsterCollisionCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterCollisionCheckBoxEvent);
}

void UMonsterDebugWidget::MonsterToggleAICheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->BroadcastToggleAI(value);
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterToggleAIDebug(MonsterAIDisableCheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterHitCheckCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterHitCheckDebug(MonsterHitCheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterAttackDebug(MonsterAttackCheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterCollisionDebug(MonsterCollisionCheckBox->IsChecked());
}
