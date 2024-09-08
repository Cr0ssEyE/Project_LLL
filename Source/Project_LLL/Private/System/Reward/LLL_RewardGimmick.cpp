// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Algo/RandomShuffle.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_AbilityDataHelper.h"

// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick() :
	TotalRewardWeight(0),
	CurrentAbilityData(nullptr),
	bIsButtonEventSetup(false),
	bMapGimmickIsExist(false),
	bIsTest(false),
	TestAbilityDataID1(110001),
	TestAbilityDataID2(110002),
	TestAbilityDataID3(110003)
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
			if (Data->ID == TestAbilityDataID1 || Data->ID == TestAbilityDataID2 || Data->ID == TestAbilityDataID3)
			{
				ButtonAbilityDataArray.Emplace(Data);
			}
		}
		RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);
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

			bool IsInValidReward = false;
			if (GettenAbilityArray.IsEmpty())
			{
				if (Reward.Key->RequireCategory != EAbilityCategory::Null)
				{
					CurrentWeight += Reward.Value;
					continue;
				}
			}
			else
			{
				for (const auto GottenReward : GettenAbilityArray)
				{
					if (Reward.Key->AbilityName == GottenReward->AbilityName)
					{
						IsInValidReward = true;
						break;
					}

					// 획득 조건 체크
					if (Reward.Key->RequireCategory == EAbilityCategory::Null)
					{
						continue;
					}

					IsInValidReward = false;
					if (Reward.Key->RequireCategory == GottenReward->AbilityCategory)
					{
						IsInValidReward = true;
						break;
					}
				}
			}

			if (IsInValidReward)
			{
				CurrentWeight += Reward.Value;
				continue;
			}
			
			if (!ButtonAbilityDataArray.IsEmpty())
			{
				for (const auto EmplacedReward : ButtonAbilityDataArray)
				{
					if (Reward.Key->AbilityName == EmplacedReward->AbilityName)
					{
						IsInValidReward = true;
						break;
					}
				}
			}

			if (IsInValidReward)
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
	if (ButtonAbilityDataArray.IsEmpty())
	{
		return;
	}
	
	ClickButtonEvent(ButtonAbilityDataArray[0]);
	GettenAbilityArray.Emplace(ButtonAbilityDataArray[0]);
	if (!bIsTest)
	{
		ButtonAbilityDataArray.Empty();
	}
}

void ALLL_RewardGimmick::ClickSecondButton()
{
	if (ButtonAbilityDataArray.IsEmpty())
	{
		return;
	}
	
	ClickButtonEvent(ButtonAbilityDataArray[1]);
	GettenAbilityArray.Emplace(ButtonAbilityDataArray[1]);
	if (!bIsTest)
	{
		ButtonAbilityDataArray.Empty();
	}
}

void ALLL_RewardGimmick::ClickThirdButton()
{
	if (ButtonAbilityDataArray.IsEmpty())
	{
		return;
	}
	
	ClickButtonEvent(ButtonAbilityDataArray[2]);
	GettenAbilityArray.Emplace(ButtonAbilityDataArray[2]);
	if (!bIsTest)
	{
		ButtonAbilityDataArray.Empty();
	}
}

void ALLL_RewardGimmick::ClickButtonEvent(const FAbilityDataTable* ButtonAbilityData)
{
	CurrentAbilityData = ButtonAbilityData;
	
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
		FAsyncLoadEffectByIDDelegate AsyncLoadEffectDelegate;
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
				Append(StaticEnum<EAbilityType>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityType))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityPart>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityPart))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityRank))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityCategory>()->GetNameStringByValue(static_cast<int64>(CurrentAbilityData->AbilityCategory))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID)
{
	TArray<const FAbilityDataTable*> EqualAbilities = FLLL_AbilityDataHelper::ApplyEruriaEffect(GetWorld(), LoadedEffects, EffectID);
	if (!EqualAbilities.IsEmpty())
	{
		for (auto EqualAbility : EqualAbilities)
		{
			AbilityData.Remove(EqualAbility);
		}
	}
	
	AbilityData.Remove(CurrentAbilityData);
	/*
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
	
	bool IsCommonEffect = true;
	
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
		
		const FGameplayTagContainer TagContainer = Effect->GetAssetTags();
		if (TagContainer.HasTag(TAG_GAS_ABILITY_PART) && !TagContainer.HasTagExact(TAG_GAS_ABILITY_PART_COMMON))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 아님")));
			IsCommonEffect = false;
			TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(TagContainer);
			for (const auto EffectHandle : EffectHandles)
			{
				const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
				if (!IsValid(ActiveEffect))
				{
					continue;
				}
				
				if (CurrentAbilityData->AbilityPart == ActiveEffect->GetAbilityData()->AbilityPart)
				{
					ASC->RemoveActiveGameplayEffect(EffectHandle);
					for (auto GameplayTag : TagContainer.GetGameplayTagArray())
					{
						UE_LOG(LogTemp, Log, TEXT("- %s 태그를 가진 이펙트 삭제"), *GameplayTag.ToString());
					}
				}
			}
		}

		// 단순 수치 변화는 여기에서 적용.
		float ChangeableValue = CurrentAbilityData->AbilityValue / static_cast<uint32>(CurrentAbilityData->AbilityValueType);
		const float UnChangeableValue = CurrentAbilityData->UnchangeableValue;
		
		if (!EffectSpecHandle.Data->Def->Modifiers.IsEmpty())
		{
			switch (EffectSpecHandle.Data->Def->Modifiers[0].ModifierOp)
			{
			case EGameplayModOp::Multiplicitive:
				++ChangeableValue;
				break;
			default: // Add, Divide, Max, Override
				break;
			}
		}
		
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_CHANGEABLE_VALUE, ChangeableValue);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_UNCHANGEABLE_VALUE, UnChangeableValue);
		
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
		if (Effect->GetGrantedTags().HasTag(TAG_GAS_HAVE_CHARGED_FEATHER))
		{
			Player->StartChargeFeather();
		}
		
		// 어빌리티 부여 계열
		if (ULLL_GE_GiveAbilityComponent* AbilitiesGameplayEffectComponent = &Effect->FindOrAddComponent<ULLL_GE_GiveAbilityComponent>())
		{
			for (const auto& AbilitySpecConfig : AbilitiesGameplayEffectComponent->GetAbilitySpecConfigs())
			{
				if (const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilitySpecConfig.Ability))
				{
					// EGameplayAbilityInstancingPolicy::InstancedPerActor로 설정된 어빌리티 한정 정상작동
					Cast<ULLL_PGA_RewardAbilityBase>(Spec->GetPrimaryInstance())->SetAbilityInfo(CurrentAbilityData);
					UE_LOG(LogTemp, Log, TEXT("스펙에 접근해서 값 바꾸기 시도"));
				}
			}
		}

		const FGameplayEventData PayLoadData;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_ABILITY_PART_GRANT, PayLoadData);
		break;
	}

	// TODO: UI 관련 상호작용 구현.
	if (IsCommonEffect)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 획득")));
		PlayerUIManager->GetInventoryWidget()->SetEruriaInfo(CurrentAbilityData);
	}
	else
	{
		PlayerUIManager->GetMainEruriaWidget()->SetEruriaInfo(CurrentAbilityData);
	}

	TArray<const FAbilityDataTable*> EqualAbilities;
	uint8 Count = 2;
	for (auto Data : AbilityData)
	{
		// 전설 이누리아의 경우 For문 중단
		if (CurrentAbilityData->ID % 10000 > ABILITY_RANK_LEGEND)
		{
			EqualAbilities.Emplace(Data);
			break;
		}
		
		if (Data->ID % ABILITY_INFO_ID == CurrentAbilityData->ID % ABILITY_INFO_ID)
		{
			EqualAbilities.Emplace(Data);
			--Count;
			if (!Count)
			{
				break;
			}
		}
	}

	if (!EqualAbilities.IsEmpty())
	{
		for (auto EqualAbility : EqualAbilities)
		{
			AbilityData.Remove(EqualAbility);
		}
	}
	AbilityData.Remove(CurrentAbilityData);
	*/
	// 테이블에서 중복 보상 제거 후 가중치 재계산
	SetRewardWeight();
	
	CurrentAbilityData = nullptr;
	bIsButtonEventSetup = false;
}
