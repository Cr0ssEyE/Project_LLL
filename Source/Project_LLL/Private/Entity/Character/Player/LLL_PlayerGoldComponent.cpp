// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerGoldComponent.h"

#include "Constant/LLL_FilePath.h"
#include "UI/Entity/Character/Player/LLL_PlayerGoldWidget.h"
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
	BeforeMoneyData = 0;
	InitMoney = 0;
	GoldComponentDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GoldComponentDataAsset>(PATH_PLAYER_GOLD_COMPONENT_DATA, EAssertionLevel::Check);
	WidgetHideWaitTime = GoldComponentDataAsset->WidgetHideWaitTime;
	
	GoldWidget = CreateDefaultSubobject<ULLL_PlayerGoldWidget>(TEXT("GoldWidget"));
	GoldWidgetClass = FLLL_ConstructorHelper::FindAndGetClass<ULLL_PlayerGoldWidget>(PATH_PLAYER_GOLD_UI_WIDGET, EAssertionLevel::Check);

}

void ULLL_PlayerGoldComponent::IncreaseMoney(const float InMoney)
{
	if (bIsShowWidget)
	{
		InitMoney += InMoney;
	}
	
	if (!bIsShowWidget)
	{
		InitMoney = InMoney;
	}
	GoldWidget->UpdateInitWidget(InitMoney);
	Money += InMoney;
	FOnMoneyChanged.Broadcast(GetMoney()); 
	ShowWidget();
}

void ULLL_PlayerGoldComponent::DecreaseMoney(const float OutMoney)
{
	if (bIsShowWidget)
	{
		InitMoney -= OutMoney;
	}

	if (!bIsShowWidget)
	{
		InitMoney = -OutMoney;
	}
	GoldWidget->UpdateInitWidget(InitMoney);
	Money = Money - OutMoney >= 0 ? Money - OutMoney : 0;
	FOnMoneyChanged.Broadcast(GetMoney());
	ShowWidget();
}


void ULLL_PlayerGoldComponent::LoadGoldData()
{
	GoldWidget->UpdateGoldWidget(GetMoney());
}

void ULLL_PlayerGoldComponent::SetGoldWidgetVisibility(bool Value)
{
	if (Value)
	{
		GoldWidget->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	
	GoldWidget->SetVisibility(ESlateVisibility::Hidden);
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
		GoldWidget->UpdateGoldWidget(GetMoney());
	}
}


// Called every frame
void ULLL_PlayerGoldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULLL_PlayerGoldComponent::PlayShowInitGoldWidgetAnim()
{
	if (bIsShowWidget)
	{
		return;
	}
	GoldWidget->PlayShowInitGoldAnimation();
	bIsShowWidget = 1;
}

void ULLL_PlayerGoldComponent::PlayHideInitGoldWidgetAnim()
{
	bIsShowWidget = 0;
	GoldWidget->PlayHideInitGoldAnimation();
	
	// 30프레임 기준 1프레임마다 호출
	GetWorld()->GetTimerManager().SetTimer(MoneyTextChangeHandle, this, &ULLL_PlayerGoldComponent::SetMoneyData, 0.03f, true);
}

void ULLL_PlayerGoldComponent::SetMoneyData()
{
	if (InitMoney > 0)
	{
		BeforeMoneyData += 1;
		GoldWidget->UpdateGoldWidget(BeforeMoneyData);
		
		if (BeforeMoneyData >= Money)
		{
			GetWorld()->GetTimerManager().ClearTimer(MoneyTextChangeHandle);
			GoldWidget->UpdateGoldWidget(Money);
		}
	}
	else
	{
		BeforeMoneyData -= 1;
		GoldWidget->UpdateGoldWidget(BeforeMoneyData);

		if (BeforeMoneyData <= Money)
		{
			GetWorld()->GetTimerManager().ClearTimer(MoneyTextChangeHandle);
			GoldWidget->UpdateGoldWidget(Money);
		}
	}
}

void ULLL_PlayerGoldComponent::ShowWidget()
{
	PlayShowInitGoldWidgetAnim();
	GetWorld()->GetTimerManager().SetTimer(WidgetWaitHideTimerHandle, this, &ULLL_PlayerGoldComponent::PlayHideInitGoldWidgetAnim, 0.1f, false, WidgetHideWaitTime);
}

