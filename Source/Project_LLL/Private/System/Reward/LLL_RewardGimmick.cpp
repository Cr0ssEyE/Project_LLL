// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "DataTable/LLL_TestAbilityDataTable.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLL_ConstructorHelper.h"
// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALLL_RewardGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALLL_RewardGimmick::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	const UProtoGameInstance* GameInstance = CastChecked<UProtoGameInstance>(GetGameInstance());

	TArray<FTestRewardDataTable*> LoadDataTables;
	GameInstance->GetRewardDataTable()->GetAllRows<FTestRewardDataTable>(TEXT("Failed To Load Reward Data Tables"), LoadDataTables);

	for (const FTestRewardDataTable* LoadDataTable : LoadDataTables)
	{
		FTestRewardDataTable TempDataTable;
		TempDataTable.RewardType = LoadDataTable->RewardType;
		TempDataTable.RewardValue = LoadDataTable->RewardValue;
		TempDataTable.bIsHardReward = LoadDataTable->bIsHardReward;
		RewardData.Emplace(TempDataTable);
	}

	TArray<FTestAbilityDataTable*> LoadAbilityDataTables;

	GameInstance->GetRewardDataTable()->GetAllRows<FTestAbilityDataTable>(TEXT("Failed To Ability Data Tables"), LoadAbilityDataTables);

	for (const FTestAbilityDataTable* LoadDataTable : LoadAbilityDataTables)
	{
		FTestAbilityDataTable TempDataTable;
		TempDataTable.AbilityCategory = LoadDataTable->AbilityCategory;
		TempDataTable.AbilityParts = LoadDataTable->AbilityParts;
		TempDataTable.AbilityRank = LoadDataTable->AbilityRank;
		TempDataTable.AbilityType = LoadDataTable->AbilityType;
		TempDataTable.AbilityValue = LoadDataTable->AbilityValue;
		TempDataTable.ChangeValue = LoadDataTable->ChangeValue;
		TempDataTable.RequireCategory = LoadDataTable->RequireCategory;
		
		AbilityData.Emplace(TempDataTable);
	}
}

// Called every frame
void ALLL_RewardGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALLL_RewardGimmick::SetRewardToDoor(ALLL_GateObject* Gate)
{
	uint8 Index = FMath::RandRange(0, RewardData.Num() - 1);
	
	Gate->SetGateInformation(&RewardData[Index]);
}

void ALLL_RewardGimmick::SetRewardButtons()
{
	if (AbilityData.Num() == 0)
	{
		const ULLL_PlayerUIManager* PlayerUIManager = CastChecked<ULLL_PlayerUIManager>(GetWorld()->GetFirstPlayerController()->GetPawn());
		ULLL_SelectRewardWidget* RewardWidget =  PlayerUIManager->GetSelectRewardWidget();
		RewardWidget->GetFirstButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickFirstButton);
		RewardWidget->GetSecondButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickSecondButton);
		RewardWidget->GetThirdButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickThirdButton);
	}
	//보상쪽 상세 시스템 기획이 나오면 바뀔 부분
	uint8 Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData1 = &AbilityData[Index];
	
	Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData2 = &AbilityData[Index];

	Index = FMath::RandRange(0, AbilityData.Num() - 1);
	ButtonAbilityData3 = &AbilityData[Index];
}

void ALLL_RewardGimmick::ClickFirstButton()
{
	//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼1 : ")).Append(AbilityData[0].AbilityCategory).Append(TEXT(" / ")).Append(AbilityData[0].AbilityParts).Append(AbilityData[0].AbilityRank).Append(TEXT(" / ")).Append(AbilityData[0].AbilityType));
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
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼2 : ")).Append(AbilityData[1].AbilityCategory).Append(TEXT(" / ")).Append(AbilityData[1].AbilityParts).Append(AbilityData[1].AbilityRank).Append(TEXT(" / ")).Append(AbilityData[1].AbilityType));
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
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString(TEXT("버튼3 : ")).Append(AbilityData[2].AbilityCategory).Append(TEXT(" / ")).Append(AbilityData[2].AbilityParts).Append(AbilityData[2].AbilityRank).Append(TEXT(" / ")).Append(AbilityData[2].AbilityType));
		}
	}
#endif
}

