// Fill out your copyright notice in the Description page of Project Settings.


#include "Y2024Q1_Prototyping/Public/UI/Debug/PlayerDebugWidget.h"
#include "Y2024Q1_Prototyping/Public/Game/ProtoGameInstance.h"

void UPlayerDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerMovementCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerMovementCheckBoxEvent);
	PlayerEvadeCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerEvadeCheckBoxEvent);
	PlayerSkillCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerSkillCheckBoxEvent);
	
	PlayerHitCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerHitCheckBoxEvent);
	PlayerAttackCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerAttackCheckBoxEvent);
	PlayerCollisionCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerCollisionCheckBoxEvent);

	PlayerFillHealthButton->OnClicked.AddDynamic(this, &UPlayerDebugWidget::PlayerFillHealthButtonEvent);
	PlayerCoolDownResetButton->OnClicked.AddDynamic(this, &UPlayerDebugWidget::PlayerCoolDownResetButtonEvent);
}

void UPlayerDebugWidget::PlayerMovementCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerMovementDebug(PlayerMovementCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerEvadeCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerEvadeDebug(PlayerEvadeCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerSkillCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerSkillDebug(PlayerSkillCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerHitCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerHitCheckDebug(PlayerHitCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerAttackDebug(PlayerAttackCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerCollisionDebug(PlayerCollisionCheckBox->IsChecked());
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
