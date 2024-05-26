// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_InventoryWidget.generated.h"

class UVerticalBox;
struct FAbilityDataTable;
class UImage;
class UTextBlock;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_InventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetEruriaInfo(const FAbilityDataTable* AbilityData);
	
public:
	FORCEINLINE void PlayOpenAnimation() { PlayAnimation(OpenInventory); }
	FORCEINLINE void PlayCloseAnimation() { PlayAnimation(CloseInventory); }

protected:
	void SetEruriaImage(UImage* Image, UTextBlock* TextBlock, FGameplayTag AbilityPartTag, const FAbilityDataTable* AbilityData);

protected:
	// 임시 데이터이므로 추후 테이블 연결 필요
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 아이콘(임시)")
	TArray<UTexture*> EruriaIConTextures;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, DisplayName = "이누리아 레어도 색상")
	TArray<FLinearColor> EruriaRarityColor;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CloseInventory;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UVerticalBox> FirstVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UVerticalBox> SecondVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UVerticalBox> ThirdVerticalBox;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UImage*> CommonEruriaImages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UTextBlock*> CommonEruriaLevelTexts;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> EruriaImageDynamicMaterials;

	UPROPERTY()
	TArray<FAbilityDataTable> CommonEruriaDataTables;
	
	uint32 CurrentEruriaSlot;
};
