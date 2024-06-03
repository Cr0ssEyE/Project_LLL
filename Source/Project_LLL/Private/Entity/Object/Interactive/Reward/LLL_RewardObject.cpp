// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "UI/Object/LLL_ProductObjectPriceWidget.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "UI/System/LLL_SelectRewardWidget.h"

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RewardObjectDataAsset>(PATH_REWARD_OBJECT_TEST_DATA, EAssertionLevel::Check);

	RewardMesh = RewardObjectDataAsset->StaticMesh;
	BaseMesh->SetStaticMesh(RewardMesh);
	BaseMesh->SetMaterial(0, RewardObjectDataAsset->MaterialInst);
	BaseMesh->SetCollisionProfileName(CP_OVERLAP_ALL);
	
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

void ALLL_RewardObject::SetInformation(const FRewardDataTable* Data)
{
	RewardData = Data;

	switch (RewardData->ID)
	{
			// 능력
		case 1:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->AbilityTexture);
		break;
			// 재화
		case 2:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->GoldTexture);
		break;
			// 최대 체력
		case 3:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->MaxHPTexture);
		break;
			// 능력 강화
		case 4:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->EnhanceTexture);
			break;
	default:;
	}
}

void ALLL_RewardObject::InteractiveEvent()
{
	Super::InteractiveEvent();
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ULLL_PlayerGoldComponent* PlayerGoldComponent = Player->GetGoldComponent();
	ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();
	
	FGameplayEffectContextHandle EffectContextHandle = Player->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Player);
	const FGameplayEffectSpecHandle EffectSpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(RewardObjectDataAsset->MaxHPEffect, 1.0, EffectContextHandle);
	
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
		TArray<const FRewardDataTable*> RewardDataArray = GameInstance->GetRewardDataTable();
		const uint8 Index = FMath::RandRange(0, RewardDataArray.Num() - 1);
		RewardData = RewardDataArray[Index];
	}
	switch (RewardData->ID)
	{
		// 능력
	case 1:
		InteractionDelegate.Broadcast();
		SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
		SelectRewardWidget->SetIsEnabled(true);
		SelectRewardWidget->FocusToUI();
		break;
		// 재화
	case 2:
		PlayerGoldComponent->IncreaseMoney(RewardData->Value);
		break;
		// 최대 체력
	case 3:
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("체력 보상 로직 진입"));
		if(EffectSpecHandle.IsValid())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("플레이어 체력 증가 시작"));
			Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}

		break;
		// 능력 강화
	case 4:
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckObjectActivateDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("player 어빌리티 강화"));
			}
		}
#endif
		break;
	default:;
	}
	
	Destroy();
}
