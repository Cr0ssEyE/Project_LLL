// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerGoldComponent.h"

#include "Constant/LLL_FilePath.h"
#include "UI/Player/LLL_PlayerGoldWidget.h"
#include "Util/LLL_ConstructorHelper.h"
#include "DataAsset/LLL_GoldComponentDataAsset.h"

// Sets default values for this component's properties
ULLL_PlayerGoldComponent::ULLL_PlayerGoldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Money = 0;
	
	GoldComponentDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GoldComponentDataAsset>(PATH_PLAYER_GOLD_COMPONENT_DATA, EAssertionLevel::Check);
	WidgetHideWaitTime = GoldComponentDataAsset->WidgetHideWaitTime;
	IsShowWidget = false;
	GoldWidget = CreateDefaultSubobject<ULLL_PlayerGoldWidget>(TEXT("GoldWidget"));
	GoldWidgetClass = FLLL_ConstructorHelper::FindAndGetClass<ULLL_PlayerGoldWidget>(PATH_PLAYER_GOLD_UI_WIDGET, EAssertionLevel::Check);
}


// Called when the game starts
void ULLL_PlayerGoldComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (IsValid(GoldWidgetClass))
	{
		GoldWidget = CastChecked<ULLL_PlayerGoldWidget>(CreateWidget(GetWorld(), GoldWidgetClass));
		GoldWidget->AddToViewport();
		GoldWidget->SetIsEnabled(false);
	}
}


// Called every frame
void ULLL_PlayerGoldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULLL_PlayerGoldComponent::EnableInteractionWidget()
{
	if (!GoldWidget->GetIsEnabled())
	{
		GoldWidget->PlayVisibleAnimation();
		GoldWidget->SetIsEnabled(true);
		IsShowWidget = true;
	}
}

void ULLL_PlayerGoldComponent::DisableInteractionWidget()
{
	GoldWidget->PlayHideAnimation();
	GoldWidget->SetIsEnabled(false);
	IsShowWidget = false;
}

void ULLL_PlayerGoldComponent::ShowWidget()
{
	GoldWidget->UpdateWidget(GetMoney());
	GetWorld()->GetTimerManager().SetTimer(WidgetWaitHideTimerHandle, this, &ULLL_PlayerGoldComponent::DisableInteractionWidget, 0.1f, false, WidgetHideWaitTime);
	if (!IsShowWidget)
	{
		EnableInteractionWidget();
	}
}

