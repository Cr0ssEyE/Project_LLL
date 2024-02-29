// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/CharacterDebugWidget.h"

#include "Components/CheckBox.h"

void UCharacterDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterHitCheckCheckBox->OnCheckStateChanged.AddDynamic(this, &UCharacterDebugWidget::CharacterHitCheckCheckBoxEvent);
	CharacterAttackCheckBox->OnCheckStateChanged.AddDynamic(this, &UCharacterDebugWidget::CharacterAttackCheckBoxEvent);
	CharacterCollisionCheckBox->OnCheckStateChanged.AddDynamic(this, &UCharacterDebugWidget::CharacterCollisionCheckBoxEvent);
}
