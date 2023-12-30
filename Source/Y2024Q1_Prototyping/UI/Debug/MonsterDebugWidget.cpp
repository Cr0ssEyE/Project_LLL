// Fill out your copyright notice in the Description page of Project Settings.


#include "Y2024Q1_Prototyping/UI/Debug/MonsterDebugWidget.h"

#include "Y2024Q1_Prototyping/Game/ProtoGameInstance.h"

void UMonsterDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MonsterAIDisableCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterAIDisableCheckBoxEvent);
	MonsterHitCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterHitCheckCheckBoxEvent);
	MonsterAttackCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterAttackCheckBoxEvent);
	MonsterCollisionCheckBox->OnCheckStateChanged.AddDynamic(this, &UMonsterDebugWidget::MonsterCollisionCheckBoxEvent);
}

void UMonsterDebugWidget::MonsterAIDisableCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->MonsterDisableAIDelegate.Broadcast();
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
