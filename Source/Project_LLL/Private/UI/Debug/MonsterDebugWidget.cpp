// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/MonsterDebugWidget.h"
#include "Components/CheckBox.h"
#include "Game/LLL_DebugGameInstance.h"

void UMonsterDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MonsterSpawnDataCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterSpawnDataCheckBoxEvent);
	MonsterDisableAICheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterToggleAICheckBoxEvent);

	MonsterDisableAICheckBox->SetCheckedState(ECheckBoxState::Checked);
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterToggleAIDebug(MonsterDisableAICheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterSpawnDataCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterSpawnDataDebug(MonsterSpawnDataCheckBox->IsChecked());
}

void UMonsterDebugWidget::MonsterToggleAICheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterToggleAIDebug(MonsterDisableAICheckBox->IsChecked());
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->BroadcastToggleAI();
}

void UMonsterDebugWidget::CharacterHitCheckCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterHitCheckDebug(CharacterHitCheckCheckBox->IsChecked());
}

void UMonsterDebugWidget::CharacterAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterAttackDebug(CharacterAttackCheckBox->IsChecked());
}

void UMonsterDebugWidget::CharacterCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetMonsterCollisionDebug(CharacterCollisionCheckBox->IsChecked());
}
