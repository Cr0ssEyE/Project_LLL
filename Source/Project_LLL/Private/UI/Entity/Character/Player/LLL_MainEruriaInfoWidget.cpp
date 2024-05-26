// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_MainEruriaInfoWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void ULLL_MainEruriaInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_MainEruriaInfoWidget::SetEruriaInfo(const FAbilityDataTable* AbilityData)
{
	UImage* AbilityEruriaImage = nullptr;
	UTextBlock* AbilityEruriaTextBlock = nullptr;
	FGameplayTag EruriaAbilityPartTag;
	
	switch (AbilityData->AbilityPart)
	{
	case EAbilityPart::Attack:
		AbilityEruriaImage = AttackEruriaImage;
		AbilityEruriaTextBlock = AttackEruriaLevelText;
		EruriaAbilityPartTag = TAG_GAS_ABILITY_PART_ATTACK;
		break;
	case EAbilityPart::Dash:
		AbilityEruriaImage = DashEruriaImage;
		AbilityEruriaTextBlock = DashEruriaLevelText;
		EruriaAbilityPartTag = TAG_GAS_ABILITY_PART_DASH;
		break;
	case EAbilityPart::Chase:
		AbilityEruriaImage = ChaseEruriaImage;
		AbilityEruriaTextBlock = ChaseEruriaLevelText;
		EruriaAbilityPartTag = TAG_GAS_ABILITY_PART_CHASE;
		break;
	case EAbilityPart::ComboSkill:
		AbilityEruriaImage = ComboSkillEruriaImage;
		AbilityEruriaTextBlock = ComboSkillEruriaLevelText;
		EruriaAbilityPartTag = TAG_GAS_ABILITY_PART_COMBO_SKILL;
		break;
	default: // EAbilityPart::Common
		//TODO: Inventory Widget에서 Common 이누리아 처리하기
		return;
	}

	if (IsValid(AbilityEruriaImage) && IsValid(AbilityEruriaTextBlock))
	{
		SetEruriaImage(AbilityEruriaImage, AbilityEruriaTextBlock, EruriaAbilityPartTag, AbilityData);
	}
}

void ULLL_MainEruriaInfoWidget::SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData)
{
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	TArray<FGameplayAbilitySpecHandle> PartAbilities;
	PlayerCharacter->GetAbilitySystemComponent()->FindAllAbilitiesWithTags(PartAbilities, FGameplayTagContainer(AbilityPartTag));
	if (PartAbilities.IsEmpty())
	{
		return;
	}
	
	const float AbilityLevel = PlayerCharacter->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(PartAbilities[0])->GetPrimaryInstance()->GetAbilityLevel();

	UMaterialInstanceDynamic* MaterialInstanceDynamic = Image->GetDynamicMaterial();
	if (!IsValid(MaterialInstanceDynamic))
	{
		return;
	}
	
	FAbilityLevelDisplayHelper AbilityLevelDisplayHelper;
	
	TextBlock->SetText(FText::FromString(AbilityLevelDisplayHelper.DisplayText[static_cast<uint32>(AbilityLevel) - 1]));
	MaterialInstanceDynamic->SetVectorParameterValue(UI_RARITY_COLOR, EruriaRarityColor[static_cast<uint32>(AbilityData->AbilityRank)]);
	MaterialInstanceDynamic->SetTextureParameterValue(UI_ERURIA_ICON, EruriaIConTextures[static_cast<uint32>(AbilityData->AbilityType)]);
}

