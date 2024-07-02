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
#include "Util/LLL_AbilityDataHelper.h"

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
		ensure(false);
		return;
	}

	if (IsValid(AbilityEruriaImage) && IsValid(AbilityEruriaTextBlock))
	{
		SetEruriaImage(AbilityEruriaImage, AbilityEruriaTextBlock, EruriaAbilityPartTag, AbilityData);
	}
}

void ULLL_MainEruriaInfoWidget::RemoveEruriaInfo(const EAbilityPart AbilityPart)
{
	UImage* AbilityEruriaImage = nullptr;
	UTextBlock* AbilityEruriaTextBlock = nullptr;

	if (AbilityPart == EAbilityPart::Common)
	{
		ensure(false);
		return;
	}
	
	switch (AbilityPart)
	{
	case EAbilityPart::Attack:
		AbilityEruriaImage = AttackEruriaImage;
		AbilityEruriaTextBlock = AttackEruriaLevelText;
		break;
	case EAbilityPart::Dash:
		AbilityEruriaImage = DashEruriaImage;
		AbilityEruriaTextBlock = DashEruriaLevelText;
		break;
	case EAbilityPart::Chase:
		AbilityEruriaImage = ChaseEruriaImage;
		AbilityEruriaTextBlock = ChaseEruriaLevelText;
		break;
	case EAbilityPart::ComboSkill:
		AbilityEruriaImage = ComboSkillEruriaImage;
		AbilityEruriaTextBlock = ComboSkillEruriaLevelText;
		break;
	default:
		checkNoEntry();
	}

	UMaterialInstanceDynamic* MaterialInstanceDynamic = AbilityEruriaImage->GetDynamicMaterial();
	if (!IsValid(MaterialInstanceDynamic) || !IsValid(AbilityEruriaImage) || !IsValid(AbilityEruriaTextBlock))
	{
		ensure(false);
		return;
	}
	
	FAbilityLevelDisplayHelper AbilityLevelDisplayHelper;
	
	AbilityEruriaTextBlock->SetText(FText::FromString(AbilityLevelDisplayHelper.DisplayText[0]));
	MaterialInstanceDynamic->SetVectorParameterValue(UI_RARITY_COLOR, EruriaRarityColor[0]);
	MaterialInstanceDynamic->SetTextureParameterValue(UI_ERURIA_ICON, EruriaIConTextures[0]);
}

void ULLL_MainEruriaInfoWidget::SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData)
{
	if (!IsValid(Image) || !IsValid(TextBlock))
	{
		ensure(false);
		return;
	}
	
	const ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!IsValid(PlayerCharacter))
	{
		ensure(false);
		return;
	}
	
	TArray<FGameplayAbilitySpecHandle> PartAbilities;
	PlayerCharacter->GetAbilitySystemComponent()->FindAllAbilitiesWithTags(PartAbilities, FGameplayTagContainer(AbilityPartTag));
	if (PartAbilities.IsEmpty())
	{
		ensure(false);
		return;
	}
	
	const float AbilityLevel = PlayerCharacter->GetAbilitySystemComponent()->FindAbilitySpecFromHandle(PartAbilities[0])->GetPrimaryInstance()->GetAbilityLevel();

	UMaterialInstanceDynamic* MaterialInstanceDynamic = Image->GetDynamicMaterial();
	if (!IsValid(MaterialInstanceDynamic))
	{
		ensure(false);
		return;
	}
	
	FAbilityLevelDisplayHelper AbilityLevelDisplayHelper;
	
	TextBlock->SetText(FText::FromString(AbilityLevelDisplayHelper.DisplayText[static_cast<uint32>(AbilityLevel)]));
	MaterialInstanceDynamic->SetVectorParameterValue(UI_RARITY_COLOR, EruriaRarityColor[static_cast<uint32>(AbilityData->AbilityRank)]);
	MaterialInstanceDynamic->SetTextureParameterValue(UI_ERURIA_ICON, EnhancedEruriaIConTextures[static_cast<uint32>(AbilityData->AbilityType) - 1].AbilityIcon[static_cast<uint32>(AbilityData->AbilityPart) - 1]);
}

// 만약 여기서 중단점 터지면 부여한 어빌리티에 태그 뭔가 빠졌는지 체크

