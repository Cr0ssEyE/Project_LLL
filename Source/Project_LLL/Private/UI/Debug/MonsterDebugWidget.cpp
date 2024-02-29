// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/MonsterDebugWidget.h"
#include "Components/CheckBox.h"
#include "Game/ProtoGameInstance.h"

void UMonsterDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MonsterSpawnDataCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterSpawnDataCheckBoxEvent);
	MonsterDisableAICheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterToggleAICheckBoxEvent);

	MonsterDisableAICheckBox->SetCheckedState(ECheckBoxState::Checked);
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterToggleAIDebug(MonsterDisableAICheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterSpawnDataCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterSpawnDataDebug(MonsterSpawnDataCheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterToggleAICheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterToggleAIDebug(MonsterDisableAICheckBox->IsChecked());
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->BroadcastToggleAI();
}

void UMonsterDebugWidget::CharacterHitCheckCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterHitCheckDebug(CharacterHitCheckCheckBox->IsChecked());
}

void UMonsterDebugWidget::CharacterAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterAttackDebug(CharacterAttackCheckBox->IsChecked());
}

void UMonsterDebugWidget::CharacterCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetMonsterCollisionDebug(CharacterCollisionCheckBox->IsChecked());
}
