// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Algo/RandomShuffle.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"
#include "Util/LLL_AbilityDataHelper.h"

// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick() :
	TotalRewardWeight(0),
	bIsButtonEventSetup(false),
	bMapGimmickIsExist(false),
	bIsTest(false)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ALLL_RewardGimmick::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		if (!bMapGimmickIsExist)
		{
			SetRewardButtons();
		}
	}));

	if (bIsTest && TestAbilityDataID.IsEmpty())
	{
		FTestAbilityDataID TempTestAbilityDataID;
		TempTestAbilityDataID.TestAbilityDataID1 = 11001;
		TempTestAbilityDataID.TestAbilityDataID2 = 11002;
		TempTestAbilityDataID.TestAbilityDataID3 = 11003;

		TestAbilityDataID.Emplace(TempTestAbilityDataID);
	}

	Cast<ULLL_GameInstance>(GetGameInstance())->RewardGimmick = this;
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_RewardGimmick::WaitPlayerInitialize);
	SetDataTable();
}

// Called every frame
void ALLL_RewardGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALLL_RewardGimmick::SetRewardToGate(ALLL_GateObject* Gate)
{
	const uint8 Index = FMath::RandRange(0, RewardData.Num() - 1);
	
	Gate->SetGateInformation(RewardData[Index]);
}

void ALLL_RewardGimmick::SetRewardButtons()
{
	if (RewardData.IsEmpty() || AbilityData.IsEmpty())
	{
		SetDataTable();
	}
	SetRewardWeight();
	Algo::RandomShuffle(NormalizedWeightRewardArray);
	
	if (bIsButtonEventSetup && !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		ensure(false);
		return;
	}

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
    ULLL_SelectRewardWidget* RewardWidget = PlayerUIManager->GetSelectRewardWidget();

	if (!RewardWidget->GetFirstButton()->OnClicked.IsAlreadyBound(this, &ALLL_RewardGimmick::ClickFirstButton))
	{
		RewardWidget->GetFirstButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickFirstButton);
	}
	
	if (!RewardWidget->GetSecondButton()->OnClicked.IsAlreadyBound(this, &ALLL_RewardGimmick::ClickSecondButton))
	{
		RewardWidget->GetSecondButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickSecondButton);
	}

	if (!RewardWidget->GetThirdButton()->OnClicked.IsAlreadyBound(this, &ALLL_RewardGimmick::ClickThirdButton))
	{
		RewardWidget->GetThirdButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickThirdButton);
	}
	
    bIsButtonEventSetup = true;
	
	if (bIsTest)
	{
		for (auto Data : AbilityData)
		{
			int32 Index = RewardWidget->Num;
			if (Index >= TestAbilityDataID.Num())
			{
				Index = 0;
			}

			const int32 ID1 = TestAbilityDataID[Index].TestAbilityDataID1;
			const int32 ID2 = TestAbilityDataID[Index].TestAbilityDataID2;
			const int32 ID3 = TestAbilityDataID[Index].TestAbilityDataID3;
			
			if (Data->ID == ID1 || Data->ID == ID2 || Data->ID == ID3)
			{
				ButtonAbilityDataArray.Emplace(Data);
			}
		}
		
		RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);

		ButtonAbilityData1 = ButtonAbilityDataArray[0];
		ButtonAbilityData2 = ButtonAbilityDataArray[1];
		ButtonAbilityData3 = ButtonAbilityDataArray[2];
		
		ButtonAbilityDataArray.Empty();
		return;
	}

	if (NormalizedWeightRewardArray.IsEmpty())
	{
		SetRewardWeight();
	}
	
	TArray<TTuple<const FAbilityDataTable*, float>> AbilityDataTables = NormalizedWeightRewardArray;

	// 만약 이누리아 보상 리롤 정보가 세이브 되어있으면 가져와서 적용.
	// 룸 넘어갈 때 Or 상점에서 이누리아 보상 상호작용 할 때 초기화 하기 때문에 문제 아마도 없?음
	for (auto RolledAbilityID : GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()->StageInfoData.SpawnedAbilityDataIDArray)
	{
		for (auto RewardAbilityTable : AbilityData)
		{
			if (RolledAbilityID == RewardAbilityTable->ID)
			{
				ButtonAbilityDataArray.Emplace(RewardAbilityTable);
				break;
			}
		}
	}
	
	if (!ButtonAbilityDataArray.IsEmpty())
	{
		RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);
		return;
	}
	
	RollReward(AbilityDataTables);

	uint8 LoopCount = 0;
	while (ButtonAbilityDataArray.Num() < 3)
	{
		AbilityDataTables = NormalizedWeightRewardArray;
		RollReward(AbilityDataTables);
		++LoopCount;
		if (LoopCount > 1000) // 1000번 이상 돌면 문제 있?음
		{
			UE_LOG(LogTemp, Log, TEXT("가중치 리롤 1000회 이상 동작"));
			ensure(false);
			return;
		}
	}

	RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);

	ButtonAbilityData1 = ButtonAbilityDataArray[0];
	ButtonAbilityData2 = ButtonAbilityDataArray[1];
	ButtonAbilityData3 = ButtonAbilityDataArray[2];

	ButtonAbilityDataArray.Empty();
}

void ALLL_RewardGimmick::SetDataTable()
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	
	RewardData = GameInstance->GetRewardDataTable();
	AbilityData = GameInstance->GetAbilityDataTable();
}

void ALLL_RewardGimmick::SetRewardWeight()
{
	NormalizedWeightRewardArray.Empty();
	for (const auto Data : AbilityData)
	{
		TotalRewardWeight += Data->GetAbilityRate;
	}

	for (auto Data : AbilityData)
	{
		NormalizedWeightRewardArray.Emplace(TTuple<const FAbilityDataTable*, float>(Data, static_cast<float>(Data->GetAbilityRate) / static_cast<float>(TotalRewardWeight)));
	}
}

void ALLL_RewardGimmick::RollReward(TArray<TTuple<const FAbilityDataTable*, float>> AbilityDataTable)
{
	for (int i = 0; i < 3 - ButtonAbilityDataArray.Num(); ++i)
	{
		const float WeightPoint = FMath::RandRange(0.f, 1.f);
		float CurrentWeight = 0.f;
		for (auto Reward : AbilityDataTable)
		{
			if (CurrentWeight < WeightPoint)
			{
				CurrentWeight += Reward.Value;
				continue;
			}

			TArray<const FAbilityDataTable*> GottenAbilityArray = FLLL_AbilityDataHelper::GottenAbilityArray(GetWorld());
			
			bool IsValidReward = true;
			if (GottenAbilityArray.IsEmpty())
			{
				if (Reward.Key->RequireCategory != EAbilityCategory::Null)
				{
					CurrentWeight += Reward.Value;
					continue;
				}
			}
			else
			{
				for (const auto GottenReward : GottenAbilityArray)
				{
					// 획득 조건이 필요한 이누리아일 경우
					if (Reward.Key->RequireCategory != EAbilityCategory::Null)
					{
						IsValidReward = false;
						if (Reward.Key->RequireCategory == GottenReward->AbilityCategory)
						{
							IsValidReward = true;
							break;
						}
					}
				}

				if (!IsValidReward)
				{
					CurrentWeight += Reward.Value;
					continue;
				}

				for (const auto GottenReward : GottenAbilityArray)
				{
					// 중첩 이누리아일 경우
					if (Reward.Key->TagID[0] == '1')
					{
						IsValidReward = true;
						break;
					}
					if (Reward.Key->AbilityName == GottenReward->AbilityName && Reward.Key->ID <= GottenReward->ID)
					{
						IsValidReward = false;
						break;
					}
				}

				if (!IsValidReward)
				{
					CurrentWeight += Reward.Value;
					continue;
				}
			}
			
			if (!ButtonAbilityDataArray.IsEmpty())
			{
				for (const auto EmplacedReward : ButtonAbilityDataArray)
				{
					if (Reward.Key->AbilityName == EmplacedReward->AbilityName)
					{
						IsValidReward = false;
						break;
					}
				}
			}

			if (!IsValidReward)
			{
				CurrentWeight += Reward.Value;
				continue;
			}
			
			ButtonAbilityDataArray.Emplace(Reward.Key);
			break;
		}
	}
}

void ALLL_RewardGimmick::WaitPlayerInitialize()
{
	ALLL_PlayerController* PlayerController = Cast<ALLL_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(PlayerController) && PlayerController->CheckPlayerInitialized() && !bMapGimmickIsExist)
	{
		SetRewardButtons();
		return;
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_RewardGimmick::WaitPlayerInitialize);
}

void ALLL_RewardGimmick::ClickFirstButton()
{
	ClickButtonEvent(ButtonAbilityData1);
}

void ALLL_RewardGimmick::ClickSecondButton()
{
	ClickButtonEvent(ButtonAbilityData2);
}

void ALLL_RewardGimmick::ClickThirdButton()
{
	ClickButtonEvent(ButtonAbilityData3);
}

void ALLL_RewardGimmick::ClickButtonEvent(const FAbilityDataTable* ButtonAbilityData)
{
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
		FAsyncLoadEffectByIDDelegate AsyncLoadEffectDelegate;
		AsyncLoadEffectDelegate.AddDynamic(this, &ALLL_RewardGimmick::ReceivePlayerEffectsHandle);
		AbilityManageSubSystem->ASyncLoadEffectsByID(AsyncLoadEffectDelegate, EEffectOwnerType::Player, ButtonAbilityData->ID, EEffectAccessRange::None);
	}
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
				FString(TEXT("버튼 : ")).
				Append(StaticEnum<EAnimalType>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AnimalType))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityRank))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityCategory>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityCategory))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID)
{
	FLLL_AbilityDataHelper::ApplyEnuriaEffect(GetWorld(), LoadedEffects, EffectID, AbilityData, bIsTest);
	SetRewardWeight();

	ULLL_GameProgressManageSubSystem* GameProgressSubSystem = GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>();
	if (IsValid(GameProgressSubSystem))
	{
		GameProgressSubSystem->BeginSaveGame();
	}
	
	bIsButtonEventSetup = false;
}
