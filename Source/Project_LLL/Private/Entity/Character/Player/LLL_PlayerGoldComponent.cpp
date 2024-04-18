// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerGoldComponent.h"

#include "Constant/LLL_FilePath.h"
#include "UI/Player/LLL_PlayerGoldWidget.h"
#include "Util/LLLConstructorHelper.h"
#include "DataAsset/LLL_GoldComponentDataAsset.h"

// Sets default values for this component's properties
ULLL_PlayerGoldComponent::ULLL_PlayerGoldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	Money = 0;
	InitMoney = 0;
	GoldComponentDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_GoldComponentDataAsset>(PATH_PLAYER_GOLD_COMPONENT_DATA, EAssertionLevel::Check);
	WidgetHideWaitTime = GoldComponentDataAsset->WidgetHideWaitTime;
	bIsShowWidget = 0;
	GoldWidget = CreateDefaultSubobject<ULLL_PlayerGoldWidget>(TEXT("GoldWidget"));
	GoldWidgetClass = FLLLConstructorHelper::FindAndGetClass<ULLL_PlayerGoldWidget>(PATH_PLAYER_GOLD_UI_WIDGET, EAssertionLevel::Check);

}

void ULLL_PlayerGoldComponent::IncreaseMoney(const float InMoney)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("IncreaseMoney : %d"), bIsShowWidget));
	if (bIsShowWidget == 1)
	{
		InitMoney += InMoney;
	}
	
	if (bIsShowWidget == 0)
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("DecreaseMoney : %d"), bIsShowWidget));
	if (bIsShowWidget == 1)
	{
		InitMoney -= OutMoney;
	}

	if (bIsShowWidget == 0)
	{
		InitMoney = -OutMoney;
	}
	GoldWidget->UpdateInitWidget(InitMoney);
	Money = Money - OutMoney >= 0 ? Money - OutMoney : 0;
	FOnMoneyChanged.Broadcast(GetMoney());
	ShowWidget();
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("PlayShowInitGoldWidgetAnim() : %d"), bIsShowWidget));
	if (bIsShowWidget == 1)
	{
		return;
	}
	GoldWidget->PlayShowInitGoldAnimation();
	bIsShowWidget = 1;
}

void ULLL_PlayerGoldComponent::PlayHideInitGoldWidgetAnim()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("PlayHideInitGoldWidgetAnim() : %d"), bIsShowWidget));
	bIsShowWidget = 0;
	GoldWidget->PlayHideInitGoldAnimation();
	GoldWidget->UpdateGoldWidget(GetMoney());
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("PlayHideInitGoldWidgetAnim() : %d"), bIsShowWidget));
}

void ULLL_PlayerGoldComponent::ShowWidget()
{
	GoldWidget->UpdateInitWidget(GetMoney());
	PlayShowInitGoldWidgetAnim();
	GetWorld()->GetTimerManager().SetTimer(WidgetWaitHideTimerHandle, this, &ULLL_PlayerGoldComponent::PlayHideInitGoldWidgetAnim, 0.1f, false, WidgetHideWaitTime);
}

