// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_DisplaySettingWidget.generated.h"

class UCheckBox;
enum class EResolutionTypes : uint8;
class UButton;
class UComboBoxString;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_DisplaySettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplyFullScreen();

	UFUNCTION(BlueprintCallable)
	void ApplyWindowScreen();

	UFUNCTION(BlueprintCallable)
	void ApplyMaxFrameRateValue(FString FrameRateString, ESelectInfo::Type Info);
	
	UFUNCTION(BlueprintCallable)
	void ChangeVSyncState(bool IsChecked);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> UseFullScreenButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> UseWindowButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> MaxFrameRateComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UCheckBox> VSyncCheckBox;
	
};
