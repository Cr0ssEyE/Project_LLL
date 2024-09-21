// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Game/LLL_DebugGameInstance.h"
#include "AbilitySystemComponent.h"
#include "Algo/RandomShuffle.h"
#include "Constant/LLL_AbilityRealNumbers.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "GAS/Effect/LLL_GE_GiveAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"

// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick() :
	TotalRewardWeight(0),
	CurrentAbilityData(nullptr),
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

void ALLL_RewardGimmick::ClickFirstButton()
{
	ClickButtonEvent(ButtonAbilityData1);
	GottenAbilityArray.Emplace(ButtonAbilityData1);
}

void ALLL_RewardGimmick::ClickSecondButton()
{
	ClickButtonEvent(ButtonAbilityData2);
	GottenAbilityArray.Emplace(ButtonAbilityData2);
}

void ALLL_RewardGimmick::ClickThirdButton()
{
	ClickButtonEvent(ButtonAbilityData3);
	GottenAbilityArray.Emplace(ButtonAbilityData3);
}

void ALLL_RewardGimmick::ClickButtonEvent(const FAbilityDataTable* ButtonAbilityData)
{
	CurrentAbilityData = ButtonAbilityData;
	
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
		FAsyncLoadEffectDelegate AsyncLoadEffectDelegate;
		AsyncLoadEffectDelegate.AddDynamic(this, &ALLL_RewardGimmick::ReceivePlayerEffectsHandle);
		AbilityManageSubSystem->ASyncLoadEffectsByID(AsyncLoadEffectDelegate, EEffectOwnerType::Player, CurrentAbilityData->ID, EEffectAccessRange::None);
	}
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
				FString(TEXT("버튼 : ")).
				Append(StaticEnum<EAnimalType>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AnimalType))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityRank))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityCategory>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityCategory))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects)
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

	if (!IsValid(PlayerUIManager) || !IsValid(ASC))
	{
		ensure(false);
		return;
	}

	if (LoadedEffects.IsEmpty())
	{
		Player->GetGoldComponent()->IncreaseMoney(123);
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("부여 된 플레이어 이펙트"));
	for (auto& LoadedEffect : LoadedEffects)
	{
		ULLL_ExtendedGameplayEffect* Effect = CastChecked<ULLL_ExtendedGameplayEffect>(LoadedEffect.Get()->GetDefaultObject());
		Effect->SetAbilityInfo(CurrentAbilityData);

		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(Player);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(Effect->GetClass(), 1.0, EffectContextHandle);
		if(!EffectSpecHandle.IsValid())
		{
			continue;
		}

		TArray<FActiveGameplayEffectHandle> EffectHandles;
		const TArray<FActiveGameplayEffectHandle> AllowEffectHandles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_NESTING_ALLOW));
		const TArray<FActiveGameplayEffectHandle> DenyEffectHandles = ASC->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_NESTING_DENY));
		
		EffectHandles.Append(AllowEffectHandles);
		EffectHandles.Append(DenyEffectHandles);

		for (const auto EffectHandle : EffectHandles)
		{
			const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
			if (!IsValid(ActiveEffect))
			{
				continue;
			}
				
			if (CurrentAbilityData->TagID[1] == '1' && ActiveEffect->GetAbilityData()->TagID[1] == '1')
			{
				ASC->RemoveActiveGameplayEffect(EffectHandle);
				GottenAbilityArray.Remove(ActiveEffect->GetAbilityData());
				if (CurrentAbilityData->AbilityName != ActiveEffect->GetAbilityData()->AbilityName)
				{
					AbilityData.Emplace(ActiveEffect->GetAbilityData());
				}
				UE_LOG(LogTemp, Log, TEXT("사용 타입 이펙트 삭제"));
			}
			else if (CurrentAbilityData->AbilityName == ActiveEffect->GetAbilityData()->AbilityName)
			{
				ASC->RemoveActiveGameplayEffect(EffectHandle);
				GottenAbilityArray.Remove(ActiveEffect->GetAbilityData());
				UE_LOG(LogTemp, Log, TEXT("낮은 티어 이펙트 삭제"));
			}
		}

		// 단순 수치 변화는 여기에서 적용.
		float MagnitudeValue1 = CurrentAbilityData->AbilityValue1 / static_cast<uint32>(CurrentAbilityData->Value1Type);
		float MagnitudeValue2 = CurrentAbilityData->AbilityValue2 / static_cast<uint32>(CurrentAbilityData->Value2Type);
		
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, MagnitudeValue1);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, MagnitudeValue2);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_1, MagnitudeValue1 * 100.0f);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_HUNDRED_VALUE_2, MagnitudeValue2 * 100.0f);
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		
		// 어빌리티 부여 계열
		if (ULLL_GE_GiveAbilityComponent* AbilitiesGameplayEffectComponent = &Effect->FindOrAddComponent<ULLL_GE_GiveAbilityComponent>())
		{
			for (const auto& AbilitySpecConfig : AbilitiesGameplayEffectComponent->GetAbilitySpecConfigs())
			{
				if (FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilitySpecConfig.Ability))
				{
					// EGameplayAbilityInstancingPolicy::InstancedPerActor로 설정된 어빌리티 한정 정상작동
					Cast<ULLL_PGA_RewardAbilityBase>(Spec->GetPrimaryInstance())->SetAbilityInfo(CurrentAbilityData);
					UE_LOG(LogTemp, Log, TEXT("스펙에 접근해서 값 바꾸기 시도"));
				}
			}
		}
		
		UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
		break;
	}

	// TODO: UI 관련 상호작용 구현.
	PlayerUIManager->GetInventoryWidget()->SetEruriaInfo(CurrentAbilityData);
	//PlayerUIManager->GetMainEruriaWidget()->SetEruriaInfo(CurrentAbilityData);

	// 테스트 중이 아니면 테이블에서 중복 보상 제거 후 가중치 재계산
	if (!bIsTest)
	{
		// 중첩 타입 이누리아일 경우 제거하지 않기
		if (CurrentAbilityData->TagID[0] != '1')
		{
			AbilityData.Remove(CurrentAbilityData);
			SetRewardWeight();
		}
	}
	
	CurrentAbilityData = nullptr;
	bIsButtonEventSetup = false;
}
