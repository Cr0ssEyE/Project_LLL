// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_SelectRewardWidget.h"

#include "Game/ProtoGameInstance.h"

void ULLL_SelectRewardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RewardButton1 = Cast<UButton>(GetWidgetFromName(TEXT("RewardButton1")));
	ensure(RewardButton1);
	RewardButton1->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
	
	RewardButton2 = Cast<UButton>(GetWidgetFromName(TEXT("RewardButton2")));
	ensure(RewardButton2);
	RewardButton2->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
	
	RewardButton3 = Cast<UButton>(GetWidgetFromName(TEXT("RewardButton3")));
	ensure(RewardButton3);
	RewardButton3->OnClicked.AddDynamic(this, &ULLL_SelectRewardWidget::CheckButton);
}

void ULLL_SelectRewardWidget::SetRewardButton()
{
		
}

void ULLL_SelectRewardWidget::CheckButton()
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("보상 버튼 입력")));
		}
	}
#endif
}
