// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include <Util/LLLConstructorHelper.h>

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardMesh = FLLLConstructorHelper::FindAndGetObject<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Props/SM_CornerFrame.SM_CornerFrame'"), EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(RewardMesh);

	SelectRewardWidget = CreateDefaultSubobject<ULLL_SelectRewardWidget>(TEXT("GoldWidget"));
	SelectRewardWidgetClass = FLLLConstructorHelper::FindAndGetClass<ULLL_SelectRewardWidget>(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/Reward/WBP_SelectRewordWidget.WBP_SelectRewordWidget_C'"), EAssertionLevel::Check);
}

void ALLL_RewardObject::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(SelectRewardWidgetClass))
	{
		SelectRewardWidget = CastChecked<ULLL_SelectRewardWidget>(CreateWidget(GetWorld(), SelectRewardWidgetClass));
		SelectRewardWidget->AddToViewport();
		SelectRewardWidget->SetVisibility(ESlateVisibility::Hidden);
		SelectRewardWidget->SetIsEnabled(false);
	}
}

void ALLL_RewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();
	SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
	SelectRewardWidget->SetIsEnabled(true);
}