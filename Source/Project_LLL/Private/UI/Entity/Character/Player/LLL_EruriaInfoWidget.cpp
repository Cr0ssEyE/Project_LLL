// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Entity/Character/Player/LLL_EruriaInfoWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "DataTable/LLL_AbilityDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Kismet/GameplayStatics.h"

void ULLL_EruriaInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULLL_EruriaInfoWidget::SetEruriaInfo(const FAbilityDataTable* AbilityData)
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
	default:
		checkNoEntry();
	}

	if (IsValid(AbilityEruriaImage) && IsValid(AbilityEruriaTextBlock))
	{
		SetEruriaImage(AbilityEruriaImage, AbilityEruriaTextBlock, EruriaAbilityPartTag, AbilityData);
	}
}

void ULLL_EruriaInfoWidget::SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData)
{
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	TArray<FGameplayAbilitySpecHandle> PartAbilities;
	PlayerCharacter->GetAbilitySystemComponent()->FindAllAbilitiesWithTags(PartAbilities, FGameplayTagContainer(AbilityPartTag));
	const float AbilityLevel = PlayerCharacter->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(PartAbilities[0])->GetPrimaryInstance()->GetAbilityLevel();

	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), static_cast<uint32>(AbilityLevel))));
	Image->GetDynamicMaterial()->SetVectorParameterValue(UI_RARITY_COLOR, EruriaRarityColor[static_cast<uint32>(AbilityData->AbilityRank)]);
	Image->GetDynamicMaterial()->SetTextureParameterValue(UI_ERURIA_ICON, EruriaIConTextures[static_cast<uint32>(AbilityData->AbilityType) - 1]);
}

