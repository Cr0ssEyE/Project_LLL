// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_SelectRewardWidget.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Constant/LLL_GeneralConstants.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_DebugGameInstance.h"

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

FReply ULLL_SelectRewardWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Tab)
	{
		Cast<ALLL_PlayerBase>(GetOwningPlayerPawn())->GetPlayerUIManager()->ToggleInventoryWidget();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Escape || InKeyEvent.GetKey() == EKeys::BackSpace)
	{
		Cast<ALLL_PlayerBase>(GetOwningPlayerPawn())->GetPlayerUIManager()->TogglePauseWidget(false);
		return FReply::Handled();
	}
	
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void ULLL_SelectRewardWidget::SetWidgetInfo(TArray<const FAbilityDataTable*> AbilityDataArray)
{
	if (AbilityDataArray.IsEmpty())
	{
		ensure(false);
		return;
	}
	
	const UDataTable* StringDataTable = GetGameInstance<ULLL_GameInstance>()->GetStringDataTable();

	TArray<TTuple<FString, FString>> WidgetInfoTexts;
	for (const auto AbilityData : AbilityDataArray)
	{
		FString AbilityName;
		FString AbilityInformation;
		if (!AbilityData->AbilityName.IsEmpty() && !AbilityData->AbilityInformation.IsEmpty())
		{
			AbilityName = StringDataTable->FindRow<FStringDataTable>(*AbilityData->AbilityName, TEXT("Failed To Load Ability Name"))->Korean;
			AbilityInformation = StringDataTable->FindRow<FStringDataTable>(*AbilityData->AbilityInformation, TEXT("Failed To Load Ability Information"))->Korean;

			const float Value1 = FMath::Abs(AbilityData->AbilityValue1);
			const float Value2 = FMath::Abs(AbilityData->AbilityValue2);
			const float RequireSynergy = FMath::Abs(AbilityData->RequireSynergy);

			FString Value1Text = static_cast<int>(Value1 * 10.0f) % 10 == 0.0f ? FString::FromInt(Value1) :  FString::SanitizeFloat(Value1);
			FString Value2Text = static_cast<int>(Value2 * 10.0f) % 10 == 0.0f ? FString::FromInt(Value2) :  FString::SanitizeFloat(Value2);
			FString RequireSynergyText = static_cast<int>(RequireSynergy * 10.0f) % 10 == 0.0f ? FString::FromInt(RequireSynergy) :  FString::SanitizeFloat(RequireSynergy);

			// TODO: 강화 UI는 AbilityData->ChangeValue 고려하도록 개선하기
			AbilityInformation = AbilityInformation.Replace(UI_ABILITY_INFO_ABILITY_VALUE_1, *Value1Text);
			AbilityInformation = AbilityInformation.Replace(UI_ABILITY_INFO_ABILITY_VALUE_2, *Value2Text);
			AbilityInformation = AbilityInformation.Replace(UI_ABILITY_INFO_REQUIRE_SYNERGY, *RequireSynergyText);
		}
		else
		{
			AbilityName = FString::FromInt(AbilityData->ID);
			AbilityInformation = FString::FromInt(AbilityData->ID);
		}
		WidgetInfoTexts.Emplace(TTuple<FString, FString>(AbilityName, AbilityInformation));
	}
	
	RewardNameText1->SetText(FText::FromString(WidgetInfoTexts[0].Key));
	RewardInfoText1->SetText(FText::FromString(WidgetInfoTexts[0].Value));
	RewardNameText1->SetDefaultColorAndOpacity(EruriaRarityColor[static_cast<uint32>(AbilityDataArray[0]->AbilityRank)]);
	RewardIconImage1->SetBrushFromTexture(EnhancedEruriaIConTextures[static_cast<uint32>(AbilityDataArray[0]->AnimalType) - 1].AbilityIcon[static_cast<uint32>(AbilityDataArray[0]->AbilityCategory)]);
	//RewardBackgroundImage1->SetBrushFromTexture(EruriaBackgroundTextures[static_cast<uint32>(AbilityDataArray[0]->AnimalType)]);
	
	RewardNameText2->SetText(FText::FromString(WidgetInfoTexts[1].Key));
	RewardInfoText2->SetText(FText::FromString(WidgetInfoTexts[1].Value));
	RewardNameText2->SetDefaultColorAndOpacity(EruriaRarityColor[static_cast<uint32>(AbilityDataArray[1]->AbilityRank)]);
	RewardIconImage2->SetBrushFromTexture(EnhancedEruriaIConTextures[static_cast<uint32>(AbilityDataArray[1]->AnimalType) - 1].AbilityIcon[static_cast<uint32>(AbilityDataArray[1]->AbilityCategory)]);
	//RewardBackgroundImage2->SetBrushFromTexture(EruriaBackgroundTextures[static_cast<uint32>(AbilityDataArray[1]->AnimalType)]);
	
	RewardNameText3->SetText(FText::FromString(WidgetInfoTexts[2].Key));
	RewardInfoText3->SetText(FText::FromString(WidgetInfoTexts[2].Value));
	RewardNameText3->SetDefaultColorAndOpacity(EruriaRarityColor[static_cast<uint32>(AbilityDataArray[2]->AbilityRank)]);
	RewardIconImage3->SetBrushFromTexture(EnhancedEruriaIConTextures[static_cast<uint32>(AbilityDataArray[2]->AnimalType) - 1].AbilityIcon[static_cast<uint32>(AbilityDataArray[2]->AbilityCategory)]);
	//RewardBackgroundImage3->SetBrushFromTexture(EruriaBackgroundTextures[static_cast<uint32>(AbilityDataArray[2]->AnimalType)]);
}

void ULLL_SelectRewardWidget::FocusToUI()
{
	SetKeyboardFocus();
	Cast<ALLL_PlayerController>(GetOwningPlayer())->SetUIInputMode(GetCachedWidget());
}

void ULLL_SelectRewardWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == RewardOneSelect || Animation == RewardTwoSelect || Animation == RewardThreeSelect)
	{
		SetIsEnabled(false);
		SetVisibility(ESlateVisibility::Hidden);
		Cast<ALLL_PlayerController>(GetOwningPlayer())->SetGameInputMode();
		PlayAnimationForward(ResetState);
	}
}
