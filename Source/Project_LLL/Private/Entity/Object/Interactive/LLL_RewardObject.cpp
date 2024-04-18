// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include <Util/LLL_ConstructorHelper.h>

#include "Constant/LLL_FilePath.h"

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardMesh = FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_REWARD_OBJECT_TEST_MESH, EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(RewardMesh);

	SelectRewardWidget = CreateDefaultSubobject<ULLL_SelectRewardWidget>(TEXT("GoldWidget"));
	SelectRewardWidgetClass = FLLL_ConstructorHelper::FindAndGetClass<ULLL_SelectRewardWidget>(PATH_REWARD_UI_WIDGET, EAssertionLevel::Check);
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
	
	Destroy();
}