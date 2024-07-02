// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_MainEruriaInfoWidget.generated.h"

struct FAbilityIconWrapper;
enum class EAbilityPart : uint8;
struct FGameplayTag;
struct FAbilityDataTable;
class UTextBlock;
class UImage;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FAbilityLevelDisplayHelper
{
	GENERATED_BODY()

	FAbilityLevelDisplayHelper()
	{
		DisplayText.Emplace("-");
		DisplayText.Emplace("I");
		DisplayText.Emplace("II");
		DisplayText.Emplace("III");
		DisplayText.Emplace("IV");
		DisplayText.Emplace("V");
		DisplayText.Emplace("VI");
		DisplayText.Emplace("VII");
		DisplayText.Emplace("VIII");
		DisplayText.Emplace("IX");
		DisplayText.Emplace("X");
		DisplayText.Emplace("X+");
	}

	TArray<FString> DisplayText;
};

UCLASS()
class PROJECT_LLL_API ULLL_MainEruriaInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetEruriaInfo(const FAbilityDataTable* AbilityData);
	void RemoveEruriaInfo(const EAbilityPart AbilityPart);
	
protected:
	void SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData);
	
protected:
	// 임시 데이터이므로 추후 테이블 연결 필요
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 아이콘(임시)")
	TArray<UTexture*> EruriaIConTextures;

	// 3x4 AbilityType - AbilityPart
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 아이콘")
	TArray<FAbilityIconWrapper> EnhancedEruriaIConTextures;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 레어도 색상")
	TArray<FLinearColor> EruriaRarityColor;
	
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 이미지 베이스 머티리얼")
	// TObjectPtr<UMaterial> EruriaImageMaterial;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> AttackEruriaImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> AttackEruriaLevelText;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> DashEruriaImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> DashEruriaLevelText;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ChaseEruriaImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ChaseEruriaLevelText;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UImage> ComboSkillEruriaImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> ComboSkillEruriaLevelText;
};
