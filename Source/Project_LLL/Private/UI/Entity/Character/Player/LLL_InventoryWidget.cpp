// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_InventoryWidget.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GAS/Ability/Character/Player/RewardAbilitiesList/Base/LLL_PGA_RewardAbilityBase.h"
#include "GAS/Effect/LLL_ExtendedGameplayEffect.h"
#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"

void ULLL_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterInventoryLayout(FirstVerticalBox);
	RegisterInventoryLayout(SecondVerticalBox);
	RegisterInventoryLayout(ThirdVerticalBox);

	CurrentEmptyEruriaSlotIndex = 0;
}

void ULLL_InventoryWidget::SetEruriaInfo(const FAbilityDataTable* AbilityData)
{
	if (SetEruriaImage(CommonEruriaImages[CurrentEmptyEruriaSlotIndex], CommonEruriaLevelTexts[CurrentEmptyEruriaSlotIndex], AbilityData))
	{
		CurrentEmptyEruriaSlotIndex++;
	}
}

void ULLL_InventoryWidget::RegisterInventoryLayout(const UVerticalBox* VerticalBox)
{
	if (!VerticalBox->HasAnyChildren())
	{
		return;
	}

	TArray<UCanvasPanel*> CanvasPanelWidgets;
	for (const auto Widget : VerticalBox->GetAllChildren())
	{
		if (UCanvasPanel* CanvasPanelWidget = Cast<UCanvasPanel>(Widget))
		{
			CanvasPanelWidgets.Emplace(CanvasPanelWidget);
		}
	}

	if (CanvasPanelWidgets.IsEmpty())
	{
		ensure(false);
		return;
	}
	
	for (const auto Widget : CanvasPanelWidgets)
	{
		if (UImage* EruriaImage = Cast<UImage>(Widget->GetChildAt(0)))
		{
			CommonEruriaImages.Emplace(EruriaImage);
		}

		if (UTextBlock* EruriaLevelText = Cast<UTextBlock>(Widget->GetChildAt(1)))
		{
			CommonEruriaLevelTexts.Emplace(EruriaLevelText);
		}
	}
}

bool ULLL_InventoryWidget::SetEruriaImage(UImage* Image, UTextBlock* TextBlock, const FAbilityDataTable* AbilityData)
{
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetOwningPlayer()->GetCharacter());
	TArray<FGameplayAbilitySpecHandle> SpecHandles;
	Player->GetAbilitySystemComponent()->FindAllAbilitiesWithTags(SpecHandles, FGameplayTagContainer(TAG_GAS_ABILITY_PART_COMMON));

	float AbilityLevel = 0.f;
	if (!SpecHandles.IsEmpty())
	{
		for (const auto AbilitySpec : SpecHandles)
		{
			const ULLL_PGA_RewardAbilityBase* RewardAbility = Cast<ULLL_PGA_RewardAbilityBase>(Player->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpec)->GetPrimaryInstance());
			if (!RewardAbility)
			{
				continue;
			}

			if (RewardAbility->GetAbilityData()->ID == AbilityData->ID)
			{
				AbilityLevel = RewardAbility->GetAbilityLevel();
				break;
			}
		}
		
	}
	else
	{
		TArray<FActiveGameplayEffectHandle> EffectHandles = Player->GetAbilitySystemComponent()->GetActiveEffectsWithAllTags(FGameplayTagContainer(TAG_GAS_ABILITY_PART_COMMON));

		if (EffectHandles.IsEmpty())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("어빌리티 및 이펙트에서 보상 찾기 실패")));
			//ensure(false);
			return false;
		}

		for (auto EffectHandle : EffectHandles)
		{
			const FActiveGameplayEffect* ActiveGameplayEffect = Player->GetAbilitySystemComponent()->GetActiveGameplayEffect(EffectHandle);
			if (!ActiveGameplayEffect)
			{
				continue;
			}

			const ULLL_ExtendedGameplayEffect* ExtendedGameplayEffect = Cast<ULLL_ExtendedGameplayEffect>(ActiveGameplayEffect->Spec.Def);
			if (!IsValid(ExtendedGameplayEffect))
			{
				continue;
			}

			if (ExtendedGameplayEffect->GetAbilityData()->ID == AbilityData->ID)
			{
				AbilityLevel = ActiveGameplayEffect->Spec.GetLevel();
				break;
			}
		}
	}

	// AbilityLevel == 0.f -> RewardAbility == nullptr
	if (AbilityLevel == 0.f)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("스펙은 찾았는데 리워드 어빌리티 아님")));
		//ensure(false);
		return false;
	}

	UMaterialInstanceDynamic* MaterialInstanceDynamic = Image->GetDynamicMaterial();
	if (!IsValid(MaterialInstanceDynamic))
	{
		ensure(false);
		return false;
	}
	
	FAbilityLevelDisplayHelper AbilityLevelDisplayHelper;
	
	TextBlock->SetText(FText::FromString(AbilityLevelDisplayHelper.DisplayText[static_cast<uint32>(AbilityLevel)]));
	MaterialInstanceDynamic->SetVectorParameterValue(UI_RARITY_COLOR, EruriaRarityColor[static_cast<uint32>(AbilityData->AbilityRank)]);
	MaterialInstanceDynamic->SetTextureParameterValue(UI_ERURIA_ICON, EruriaIConTextures[static_cast<uint32>(AbilityData->AbilityType)]);
	return true;
}
