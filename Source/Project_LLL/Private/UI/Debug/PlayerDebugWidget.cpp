// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/PlayerDebugWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UPlayerDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerMovementCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerMovementCheckBoxEvent);
	PlayerDashCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerDashCheckBoxEvent);

	PlayerFillHealthButton->OnClicked.AddDynamic(this, &UPlayerDebugWidget::PlayerFillHealthButtonEvent);
	PlayerInvincibleCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerInvincibleCheckBoxEvent);
}

void UPlayerDebugWidget::PlayerMovementCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerMovementDebug(PlayerMovementCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerDashCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerDashDebug(PlayerDashCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterHitCheckCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerHitCheckDebug(CharacterHitCheckCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterAttackCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerAttackDebug(CharacterAttackCheckBox->IsChecked());
}

void UPlayerDebugWidget::CharacterCollisionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerCollisionDebug(CharacterCollisionCheckBox->IsChecked());
}

void UPlayerDebugWidget::PlayerFillHealthButtonEvent()
{
	// TODO: 플레이어 클래스 만들고 처리
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(!IsValid(Player))
	{
		return;
	}
	
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();
	if(!IsValid(ASC))
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(FillHealthEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void UPlayerDebugWidget::PlayerInvincibleCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->SetPlayerInvincibleMode(PlayerInvincibleCheckBox->IsChecked());
}
