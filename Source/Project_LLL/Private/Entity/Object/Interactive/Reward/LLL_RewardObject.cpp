// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"

#include "Util/LLL_ConstructorHelper.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "UI/Object/LLL_ProductObjectPriceWidget.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "UI/System/LLL_SelectRewardWidget.h"

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RewardObjectDataAsset>(PATH_REWARD_OBJECT_TEST_DATA, EAssertionLevel::Check);

	RewardMesh = RewardObjectDataAsset->StaticMesh; 
	BaseMesh->SetStaticMesh(RewardMesh);

	PriceWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("PriceWidgetComponent");
	PriceWidgetComponent->SetupAttachment(RootComponent);
	PriceWidget = CreateDefaultSubobject<ULLL_ProductObjectPriceWidget>(TEXT("PriceWidget"));

	Price = RewardObjectDataAsset->Price;
}

void ALLL_RewardObject::BeginPlay()
{
	Super::BeginPlay();
	
	PriceWidgetClass = RewardObjectDataAsset->PriceWidgetClass;
	PriceWidget = CastChecked<ULLL_ProductObjectPriceWidget>(CreateWidget(GetWorld(), PriceWidgetClass));
	
	PriceWidgetComponent->SetWidget(PriceWidget);
	PriceWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PriceWidgetComponent->SetRelativeLocation(RewardObjectDataAsset->PriceWidgetLocation);
	PriceWidgetComponent->SetDrawSize(RewardObjectDataAsset->PriceWidgetSize);
	PriceWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PriceWidget->SetVisibility(ESlateVisibility::Hidden);
	PriceWidget->SetIsEnabled(false);
}

void ALLL_RewardObject::ApplyProductEvent()
{
	bIsProduct = 1;

	PriceWidget->SetVisibility(ESlateVisibility::Visible);
	PriceWidget->SetIsEnabled(true);
	PriceWidget->SetPrice(Price);
}

void ALLL_RewardObject::SetInformation(FRewardDataTable* Data)
{
	RewardData = Data;
}

void ALLL_RewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("보상 아이디 : %f"), RewardData->ID));
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ULLL_PlayerGoldComponent* PlayerGoldComponent = Player->GetGoldComponent();
	ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();
	if (bIsProduct && PlayerGoldComponent->GetMoney() < Price)
	{
		//구매 불가능 UI 생성
		return;
	}
	if (bIsProduct)
	{
		PlayerGoldComponent->DecreaseMoney(Price);
	}
	
	if (!RewardData)
	{
		const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
		TArray<FRewardDataTable> RewardDataArray = GameInstance->GetRewardDataTable();
		const uint8 Index = FMath::RandRange(0, RewardDataArray.Num() - 1);
		RewardData = &RewardDataArray[Index];
	}
	switch (RewardData->ID)
	{
	case 1:
		SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
		SelectRewardWidget->SetIsEnabled(true);
		SelectRewardWidget->FocusToUI();
		break;
	case 2:
		PlayerGoldComponent->IncreaseMoney(RewardData->Value);
		break;
	case 3:
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (ProtoGameInstance->CheckObjectActivateDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("player 최대 체력 증가"));
			}
		}
#endif
		break;
	case 4:
		break;
	default:;
	}
	
	Destroy();
}
