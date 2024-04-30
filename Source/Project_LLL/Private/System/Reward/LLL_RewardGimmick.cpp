// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Reward/LLL_RewardGimmick.h"

#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Gate//LLL_GateObject.h"
#include "UI/System/LLL_SelectRewardWidget.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Game/ProtoGameInstance.h"
#include "AbilitySystemComponent.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"

// Sets default values
ALLL_RewardGimmick::ALLL_RewardGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ButtonAbilityData1 = nullptr;
	ButtonAbilityData2 = nullptr;
	ButtonAbilityData3 = nullptr;
	
	bIsButtonEventSetup = false;
	bMapGimmickIsExist = false;
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
	
	if (!bIsButtonEventSetup)
	{
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
		const ULLL_PlayerUIManager* PlayerUIManager = Player->GetPlayerUIManager();
		const ULLL_SelectRewardWidget* RewardWidget = PlayerUIManager->GetSelectRewardWidget();
		RewardWidget->GetFirstButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickFirstButton);
		RewardWidget->GetSecondButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickSecondButton);
		RewardWidget->GetThirdButton()->OnClicked.AddDynamic(this, &ALLL_RewardGimmick::ClickThirdButton);
		bIsButtonEventSetup = true;
	}

	//보상쪽 상세 시스템 기획이 나오면 바뀔 부분
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

void ALLL_RewardGimmick::ClickButtonEvent(FAbilityDataTable* ButtonAbilityData) const
{
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetWorld()->GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
		FAsyncLoadEffectDelegate AsyncLoadEffectDelegate;
		AsyncLoadEffectDelegate.AddDynamic(this, &ALLL_RewardGimmick::ReceivePlayerEffectsHandle);
		AbilityManageSubSystem->ASyncLoadEffectsByID(AsyncLoadEffectDelegate, EEffectOwnerType::Player, ButtonAbilityData->ID, EEffectAccessRange::None);
	}
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckObjectActivateDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange,
				FString(TEXT("버튼1 : ")).
				Append(StaticEnum<EAbilityType>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityType))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityPart>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityPart))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityRank>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityRank))).
				Append(TEXT(" / ")).
				Append(StaticEnum<EAbilityCategory>()->GetNameStringByValue(static_cast<int64>(ButtonAbilityData->AbilityCategory))));
		}
	}
#endif
}

void ALLL_RewardGimmick::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UAbilitySystemComponent* ASC = Player->GetAbilitySystemComponent();

	UE_LOG(LogTemp, Log, TEXT("부여 된 플레이어 이펙트"));
	for (auto LoadedEffect : LoadedEffects)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(Player);

		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(LoadedEffect.Get(), 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ULLL_ExtendedGameplayEffect* Effect = CastChecked<ULLL_ExtendedGameplayEffect>(LoadedEffect->GetDefaultObject());
			Effect->SetAbilityRank(EAbilityRank::Legend);
			Effect->SendRankToAbility();
			
			const FGameplayTagContainer TagContainer = Effect->GetAssetTags();
			TArray<FActiveGameplayEffectHandle> EffectHandles = ASC->GetActiveEffectsWithAllTags(TagContainer);
			for (auto EffectHandle : EffectHandles)
			{
				if (EffectHandle.IsValid())
				{
					ASC->RemoveActiveGameplayEffect(EffectHandle);
					UE_LOG(LogTemp, Log, TEXT("- %s 삭제"), *EffectHandle.ToString());
				}
			}
		
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
			UE_LOG(LogTemp, Log, TEXT("- %s 부여"), *LoadedEffect.Get()->GetName());
		}
	}
}
