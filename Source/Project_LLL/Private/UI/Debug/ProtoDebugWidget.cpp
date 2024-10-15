// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Debug/ProtoDebugWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Enumeration/LLL_UserInterfaceEnumHelper.h"
#include "Game/LLL_AbilityManageSubSystem.h"
#include "Game/LLL_DebugGameInstance.h"
#include "System/Reward/LLL_RewardGimmickSubsystem.h"
#include "Util/LLL_AbilityDataHelper.h"

void UProtoDebugWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnablePlayerDebugWidget);
	MonsterDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableMonsterDebugWidget);
	ObjectDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::EnableObjectDebugWidget);
	SoundDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::ToggleSoundMessage);
	AllEnuriaDebugButton->OnClicked.AddDynamic(this, &UProtoDebugWidget::AllEnuriaDebug);
	SetIsEnabled(false);
	SetVisibility(ESlateVisibility::Hidden);
}

void UProtoDebugWidget::EnablePlayerDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Player));
}

void UProtoDebugWidget::EnableMonsterDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Monster));
}

void UProtoDebugWidget::EnableObjectDebugWidget()
{
	DebugWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(EDebugWidgetType::Object));
}

void UProtoDebugWidget::ToggleWidget()
{
	SetIsEnabled(!GetIsEnabled());
	GetIsEnabled() == 0 ? SetVisibility(ESlateVisibility::Hidden) : SetVisibility(ESlateVisibility::Visible);
}

void UProtoDebugWidget::ToggleSoundMessage()
{
	GetWorld()->GetGameInstanceChecked<ULLL_DebugGameInstance>()->ToggleSoundMessageDebug();
}

void UProtoDebugWidget::AllEnuriaDebug()
{
	ULLL_AbilityManageSubSystem* AbilityManageSubSystem = GetGameInstance()->GetSubsystem<ULLL_AbilityManageSubSystem>();
	if (IsValid(AbilityManageSubSystem))
	{
		//플레이어에게 AbilityData에 따라서 Tag 또는 GA 부여
		FAsyncLoadEffectByIDDelegate AsyncLoadEffectDelegate;
		AsyncLoadEffectDelegate.AddDynamic(this, &UProtoDebugWidget::ReceivePlayerEffectsHandle);

		int32 LastID = 0;
		TArray<const FAbilityDataTable*> TempAbilityData = CastChecked<ULLL_GameInstance>(GetGameInstance())->GetAbilityDataTable();
		for (int32 i = TempAbilityData.Num() - 1; i >= 0; --i)
		{
			if (LastID % 100 == TempAbilityData[i]->ID % 100)
			{
				continue;
			}

			if (TempAbilityData[i]->TagID[0] == '1' || TempAbilityData[i]->TagID[1] == '1')
			{
				continue;
			}
			
			LastID = TempAbilityData[i]->ID;
			AbilityManageSubSystem->ASyncLoadEffectsByID(AsyncLoadEffectDelegate, EEffectOwnerType::Player, LastID, EEffectAccessRange::None);
		}

		UE_LOG(LogTemp, Log, TEXT("구현된 이누리아 전부 적용 (중첩 타입, 사용 타입 제외)"))
	}
}

void UProtoDebugWidget::ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID)
{
	TArray<const FAbilityDataTable*> TempAbilityData = CastChecked<ULLL_GameInstance>(GetGameInstance())->GetAbilityDataTable();
	const ULLL_RewardGimmickSubsystem* RewardGimmickSubsystem = CastChecked<ULLL_GameInstance>(GetGameInstance())->GetSubsystem<ULLL_RewardGimmickSubsystem>();

	if (IsValid(RewardGimmickSubsystem))
	{
		FLLL_AbilityDataHelper::ApplyEnuriaEffect(GetWorld(), LoadedEffects, EffectID, TempAbilityData, RewardGimmickSubsystem->IsTest());
	}
	else
	{
		FLLL_AbilityDataHelper::ApplyEnuriaEffect(GetWorld(), LoadedEffects, EffectID, TempAbilityData, false);
	}
}
