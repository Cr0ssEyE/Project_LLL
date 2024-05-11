// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_GameplaySettingWidget.generated.h"

class UButton;
class UComboBoxString;
class UCheckBox;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GameplaySettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void OpenInputSettingWidget();
	
	UFUNCTION(BlueprintCallable)
	void ApplyGameLanguage(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplySubtitleLanguage(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplySubtitleOpacity(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplySubtitleSpeed(FString ResolutionName, ESelectInfo::Type Info);
	
	UFUNCTION(BlueprintCallable)
	void ToggleCameraShake(bool Value);

	UFUNCTION(BlueprintCallable)
	void ToggleTargetingCorrection(bool Value);
	
protected:
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	// TObjectPtr<UUserWidget> InputSettingWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> InputSettingButton;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> GameLanguageComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> SubtitleLanguageComboBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> SubtitleOpacityComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> SubtitleSpeedComboBox;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> CameraShakeActivateCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> TargetingCorrectionActivateCheckBox;

	
};
