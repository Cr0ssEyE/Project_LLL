// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_SelectRewardWidget.h"

#include "Components/RichTextBlock.h"
#include "Game/ProtoGameInstance.h"

void ULLL_SelectRewardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RewardButton1->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardOneSelectAnimation);
	RewardButton1->OnHovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardOneHoverAnimation);
	RewardButton1->OnUnhovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardOneUnHoverAnimation);
	
	RewardButton2->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardTwoSelectAnimation);
	RewardButton2->OnHovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardTwoHoverAnimation);
	RewardButton2->OnUnhovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardTwoUnHoverAnimation);
	
	RewardButton3->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardThreeSelectAnimation);
	RewardButton3->OnHovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardThreeHoverAnimation);
	RewardButton3->OnUnhovered.AddDynamic(this, &ULLL_SelectRewardWidget::PlayRewardThreeUnHoverAnimation);
}

void ULLL_SelectRewardWidget::SetWidgetInfo(TArray<FAbilityDataTable*> AbilityDataArray)
{
	const UDataTable* StringDataTable = GetGameInstance<ULLL_GameInstance>()->GetStringDataTable();

	TArray<TTuple<FString, FString>> WidgetInfoTexts;
	for (auto AbilityData : AbilityDataArray)
	{
		FString AbilityName = StringDataTable->FindRow<FStringDataTable>(AbilityData->AbilityName, TEXT("Failed To Load Ability Name"))->Korean;
		FString AbilityInformation = StringDataTable->FindRow<FStringDataTable>(*AbilityData->AbilityInformation, TEXT("Failed To Load Ability Name"))->Korean;
		
		// TODO: 강화 UI는 AbilityData->ChangeValue 고려하도록 개선하기
		AbilityInformation = AbilityInformation.Replace(TEXT("[AV]"), *FString::SanitizeFloat(AbilityData->AbilityValue));
		AbilityInformation = AbilityInformation.Replace(TEXT("[UV]"), *FString::SanitizeFloat(AbilityData->UnchangeableValue));
		WidgetInfoTexts.Emplace(TTuple<FString, FString>(AbilityName, AbilityInformation));
	}
	RewardNameText1->SetText(FText::FromString(WidgetInfoTexts[0].Key));
	RewardInfoText1->SetText(FText::FromString(WidgetInfoTexts[0].Value));
	
	RewardNameText2->SetText(FText::FromString(WidgetInfoTexts[1].Key));
	RewardInfoText2->SetText(FText::FromString(WidgetInfoTexts[1].Value));
	
	RewardNameText3->SetText(FText::FromString(WidgetInfoTexts[2].Key));
	RewardInfoText3->SetText(FText::FromString(WidgetInfoTexts[2].Value));

	SetFocus();
}

void ULLL_SelectRewardWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == RewardOneSelect || Animation == RewardTwoSelect || Animation == RewardThreeSelect)
	{
		SetIsEnabled(false);
		SetVisibility(ESlateVisibility::Hidden);
	}
}
