// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/PlayerDebugWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Game/ProtoGameInstance.h"

void UPlayerDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerMovementCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerMovementCheckBoxEvent);
	PlayerDashCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerDashCheckBoxEvent);
	PlayerSkillCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerSkillCheckBoxEvent);

	PlayerFillHealthButton->OnClicked.AddDynamic(this, &UPlayerDebugWidget::PlayerFillHealthButtonEvent);
	PlayerCoolDownResetButton->OnClicked.AddDynamic(this, &UPlayerDebugWidget::PlayerCoolDownResetButtonEvent);
}

void UPlayerDebugWidget::PlayerMovementCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerMovementDebug(PlayerMovementCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerDashCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerDashDebug(PlayerDashCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerSkillCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerSkillDebug(PlayerSkillCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterHitCheckCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerHitCheckDebug(CharacterHitCheckCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerAttackDebug(CharacterAttackCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerCollisionDebug(CharacterCollisionCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerFillHealthButtonEvent()
{
	// TODO: 플레이어 클래스 만들고 처리
	// Cast<>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UPlayerDebugWidget::PlayerCoolDownResetButtonEvent()
{
	// TODO: 플레이어 클래스 만들고 처리
	// Cast<>(GetWorld()->GetFirstPlayerController()->GetPawn());
}
