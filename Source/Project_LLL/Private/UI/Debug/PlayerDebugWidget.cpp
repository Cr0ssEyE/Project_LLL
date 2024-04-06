// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/PlayerDebugWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"

void UPlayerDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerMovementCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerMovementCheckBoxEvent);
	PlayerDashCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerDashCheckBoxEvent);
	PlayerWireActionCheckBox->OnCheckStateChanged.AddDynamic(this, &UPlayerDebugWidget::PlayerWireActionCheckBoxEvent);
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

void UPlayerDebugWidget::PlayerWireActionCheckBoxEvent(bool value)
{
	GetWorld()->GetGameInstanceChecked<UProtoGameInstance>()->SetPlayerWireActionDebug(PlayerWireActionCheckBox->IsChecked());
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
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
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

void UPlayerDebugWidget::PlayerCoolDownResetButtonEvent()
{
	// TODO: 플레이어 클래스 만들고 처리
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
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
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ResetCoolDownEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
