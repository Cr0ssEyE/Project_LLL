// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_PermanentEnhancementWidget.h"

#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"

void ULLL_PermanentEnhancementWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
}

FReply ULLL_PermanentEnhancementWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::BackSpace)
	{
		ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(GetOwningPlayerPawn());
		PlayerCharacter->GetController<ALLL_PlayerController>()->SetGameInputMode();
		RemoveFromParent();
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	
}
