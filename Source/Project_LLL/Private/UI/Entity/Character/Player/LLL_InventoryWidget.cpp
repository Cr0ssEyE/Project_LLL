// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"

#include "GameplayTagContainer.h"
#include "Components/VerticalBox.h"

void ULLL_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	for (auto Widget : FirstVerticalBox->GetAllChildren())
	{
		
	}
}

void ULLL_InventoryWidget::SetEruriaInfo(const FAbilityDataTable* AbilityData)
{
	
}

void ULLL_InventoryWidget::SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData)
{
	
}
