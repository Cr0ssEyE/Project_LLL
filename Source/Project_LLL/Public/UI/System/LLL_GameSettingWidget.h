// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/ComboBoxString.h"
#include "Sound/SoundClass.h"
#include "LLL_GameSettingWidget.generated.h"

enum class EResolutionTypes : uint8;
class UComboBoxString;
class USlider;
class UProgressBar;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GameSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	// TODO: FMOD 사운드 구분에 맞춰 적용하기
	UFUNCTION(BlueprintCallable)
	void ApplyMasterSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	void ApplyBGMSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	void ApplySFXSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void CloseSettingWidget() { /*(GetWorld()->GetGameInstanceChecked<UKWGameInstance>()->GetSaveSettingOption());*/ SetRenderScale(FVector2d::Zero()); }
	
	UFUNCTION(BlueprintCallable)
	void ApplyResolutionType(FString ResolutionName, ESelectInfo::Type Info);

	UFUNCTION(BlueprintCallable)
	void ApplyFullScreen();

	UFUNCTION(BlueprintCallable)
	void ApplyWindowScreen();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<USlider> MasterSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<USlider> BGMSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<USlider> SFXSoundVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UProgressBar> MasterVolumeProgressBar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UProgressBar> BGMVolumeProgressBar;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UProgressBar> SFXVolumeProgressBar;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> UseFullScreenBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> UseWindowBtn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> CloseBtn;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<USoundClass> MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<USoundClass> BGMSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundClass")
	TObjectPtr<USoundClass> SFXSoundClass;

	UPROPERTY()
	EResolutionTypes ResolutionTypes;

	UPROPERTY()
	uint8 bIsFullScreenActivate : 1;
	
};
