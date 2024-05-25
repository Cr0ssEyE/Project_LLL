// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_SelectRewardWidget.h"

#include "Components/RichTextBlock.h"
#include "Game/ProtoGameInstance.h"

void ULLL_SelectRewardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(RewardButton1);
	RewardButton1->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
	
	ensure(RewardButton2);
	RewardButton2->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
	
	ensure(RewardButton3);
	RewardButton3->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
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
}

void ULLL_SelectRewardWidget::CheckButton()
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("보상 버튼 입력")));
		}
	}
#endif
	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);

	//버튼 클릭 시 이펙트 및 애니 재생
}
