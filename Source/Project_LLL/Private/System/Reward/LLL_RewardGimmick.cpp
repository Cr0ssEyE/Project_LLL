// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Game/ProtoGameInstance.h"
#include "AbilitySystemComponent.h"
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
	ButtonAbilityData1(nullptr),
	ButtonAbilityData2(nullptr),
	ButtonAbilityData3(nullptr),
	CurrentAbilityData(nullptr),
	bIsButtonEventSetup(false),
	bMapGimmickIsExist(false),
	bIsTest(false),
	TestAbilityDataArrayNum1(0),
	TestAbilityDataArrayNum2(1),
	TestAbilityDataArrayNum3(2)
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
	
	Gate->SetGateInformation(&RewardData[Index]);
}

void ALLL_RewardGimmick::SetRewardButtons()
{
	if (RewardData.IsEmpty() || AbilityData.IsEmpty())
	{
		SetDataTable();
	}

	if (bIsButtonEventSetup && !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		ensure(false);
		return;
	}

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
    ULLL_SelectRewardWidget* RewardWidget = PlayerUIManager->GetSelectRewardWidget();
    RewardWidget->GetFirstButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickFirstButton);
    RewardWidget->GetSecondButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickSecondButton);
    RewardWidget->GetThirdButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickThirdButton);
    bIsButtonEventSetup = true;
	
	if (bIsTest)
	{
		// ButtonAbilityData1 = &AbilityData[TestAbilityDataArrayNum1];
		// ButtonAbilityData2 = &AbilityData[TestAbilityDataArrayNum2];
		// ButtonAbilityData3 = &AbilityData[TestAbilityDataArrayNum3];
		ButtonAbilityDataArray.Emplace(&AbilityData[TestAbilityDataArrayNum1]);
		ButtonAbilityDataArray.Emplace(&AbilityData[TestAbilityDataArrayNum2]);
		ButtonAbilityDataArray.Emplace(&AbilityData[TestAbilityDataArrayNum3]);
		RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);
		return;
	}
	
	//보상쪽 상세 시스템 기획이 나오면 바뀔 부분

	TArray<uint8> InstanceRewardIndexArray;
	uint8 Index;
	do
	{
		Index = FMath::RandRange(0, AbilityData.Num() - 1);
	}while (!GettenIndexArray.IsEmpty() && !GettenIndexArray.Contains(Index) && !InstanceRewardIndexArray.Contains(Index));
	
	// ButtonAbilityData1 = &AbilityData[Index];
	FirstButtonIndex = Index;
	ButtonAbilityDataArray.Emplace(&AbilityData[Index]);
	InstanceRewardIndexArray.Emplace(FirstButtonIndex);
	
	do
	{
		Index = FMath::RandRange(0, AbilityData.Num() - 1);
	}while (!GettenIndexArray.IsEmpty() && !GettenIndexArray.Contains(Index) && !InstanceRewardIndexArray.Contains(Index));
	// ButtonAbilityData2 = &AbilityData[Index];
	SecondButtonIndex = Index;
	ButtonAbilityDataArray.Emplace(&AbilityData[Index]);
	InstanceRewardIndexArray.Emplace(SecondButtonIndex);
	
	do
	{
		Index = FMath::RandRange(0, AbilityData.Num() - 1);
	}while (!GettenIndexArray.IsEmpty() && !GettenIndexArray.Contains(Index) && !InstanceRewardIndexArray.Contains(Index));
	// ButtonAbilityData3 = &AbilityData[Index];
	ThirdButtonIndex = Index;
	ButtonAbilityDataArray.Emplace(&AbilityData[Index]);
	InstanceRewardIndexArray.Emplace(ThirdButtonIndex);
	
	RewardWidget->SetWidgetInfo(ButtonAbilityDataArray);
}

void ALLL_RewardGimmick::SetDataTable()
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	
	RewardData = GameInstance->GetRewardDataTable();
	AbilityData = GameInstance->GetAbilityDataTable();
}

void ALLL_RewardGimmick::ClickFirstButton()
{
	ClickButtonEvent(ButtonAbilityDataArray[0]);
	GettenIndexArray.Emplace(FirstButtonIndex);
}

void ALLL_RewardGimmick::ClickSecondButton()
{
	ClickButtonEvent(ButtonAbilityDataArray[1]);
	GettenIndexArray.Emplace(SecondButtonIndex);
}

void ALLL_RewardGimmick::ClickThirdButton()
{
	ClickButtonEvent(ButtonAbilityDataArray[2]);
	GettenIndexArray.Emplace(ThirdButtonIndex);
}

void ALLL_RewardGimmick::ClickButtonEvent(FAbilityDataTable* ButtonAbilityData)
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
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
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

void ALLL_RewardGimmick::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ULLL_PlayerUIManager* PlayerUIManager =	Player->GetPlayerUIManager();
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
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 아님")));
			IsCommonEffect = false;
			TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(TagContainer);
			for (auto EffectHandle : EffectHandles)
			{
				const ULLL_ExtendedGameplayEffect* ActiveEffect = Cast<ULLL_ExtendedGameplayEffect>(ASC->GetActiveGameplayEffect(EffectHandle)->Spec.Def);
				if (!IsValid(ActiveEffect))
				{
					continue;
				}
				
				if (EffectHandle.IsValid() && CurrentAbilityData->AbilityPart == ActiveEffect->GetAbilityData()->AbilityPart)
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
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("%s 부여"), *LoadedEffect.Get()->GetName()));
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
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("스펙에 접근해서 값 바꾸기 시도")));
				}
			}
		}
	}

	// TODO: UI 관련 상호작용 구현.
	if (IsCommonEffect)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("커먼 이펙트 획득")));
		PlayerUIManager->GetInventoryWidget()->SetEruriaInfo(CurrentAbilityData);
	}
	else
	{
		PlayerUIManager->GetMainEruriaWidget()->SetEruriaInfo(CurrentAbilityData);
	}
	
	CurrentAbilityData = nullptr;
}
