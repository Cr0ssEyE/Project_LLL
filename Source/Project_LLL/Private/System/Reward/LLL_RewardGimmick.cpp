// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_TestAbilityDataTable.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLL_ConstructorHelper.h"
#include <Entity/Character/Player/LLL_PlayerBase.h>

#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Entity/Object/Interactive/LLL_RewardObject.h"
// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsButtonEventSetup = false;
}

// Called when the game starts or when spawned
void ALLL_RewardGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_RewardGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALLL_RewardGimmick::SetRewardToGate(ALLL_GateObject* Gate)
{
	uint8 Index = FMath::RandRange(0, RewardData.Num() - 1);
	
	Gate->SetGateInformation(&RewardData[Index]);
}

void ALLL_RewardGimmick::SetAbilityToReward(ALLL_AbilityRewardObject* RewardObject)
{
	TArray<FTestAbilityDataTable> AbilityDataArray;
	do
	{
		uint8 Index = FMath::RandRange(0, AbilityData.Num() - 1);
		AbilityDataArray.Emplace(AbilityData[Index]);
	}
	while (AbilityDataArray.Num() < 3);
	RewardObject->SetAbilityData(AbilityDataArray);
}

void ALLL_RewardGimmick::SetRewardButtons()
{
	if (!bIsButtonEventSetup)
	{
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
		ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
		ULLL_SelectRewardWidget* RewardWidget = PlayerUIManager->GetSelectRewardWidget();
		RewardWidget->GetFirstButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickFirstButton);
		RewardWidget->GetSecondButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickSecondButton);
		RewardWidget->GetThirdButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickThirdButton);
		bIsButtonEventSetup = true;
	}

	//TODO: 보상 오브젝트에서 데이터를 받아와서 랜덤 처리할 예정
	uint8 Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData1 = &AbilityData[Index];
	
	Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData2 = &AbilityData[Index];

	Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData3 = &AbilityData[Index];
}

void ALLL_RewardGimmick::SetDataTable()
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	
	RewardData = GameInstance->GetRewardDataTable();

	AbilityData = GameInstance->GetAbilityDataTable();
}

void ALLL_RewardGimmick::ClickFirstButton()
{
	//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼1 : ")).Append(ButtonAbilityData1->AbilityCategory).Append(TEXT(" / ")).Append(ButtonAbilityData1->AbilityParts).Append(ButtonAbilityData1->AbilityRank).Append(TEXT(" / ")).Append(FString::FromInt(static_cast<int>(ButtonAbilityData1->AbilityType))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ClickSecondButton()
{
	//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼2 : ")).Append(ButtonAbilityData2->AbilityCategory).Append(TEXT(" / ")).Append(ButtonAbilityData2->AbilityParts).Append(ButtonAbilityData2->AbilityRank).Append(TEXT(" / ")).Append(FString::FromInt(static_cast<int>(ButtonAbilityData2->AbilityType))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ClickThirdButton()
{
	//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼3 : ")).Append(ButtonAbilityData3->AbilityCategory).Append(TEXT(" / ")).Append(ButtonAbilityData3->AbilityParts).Append(ButtonAbilityData3->AbilityRank).Append(TEXT(" / ")).Append(FString::FromInt(static_cast<int>(ButtonAbilityData3->AbilityType))));
		}
	}
#endif
}

