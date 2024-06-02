// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_SoundSettingWidget.generated.h"

class UCheckBox;
class UFMODBus;
class UProgressBar;
class USlider;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SoundSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	// TODO: FMOD 사운드 구분에 맞춰 적용하기. LLL_CustomGameUserSettings에서 해야 함?
	UFUNCTION(BlueprintCallable)
	void ApplyMasterSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	void ApplyBGMSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	void ApplySFXSliderValue(const float Value);

	UFUNCTION(BlueprintCallable)
	void ApplyMasterVolumeIgnoreState(bool Value);

	UFUNCTION(BlueprintCallable)
	void ApplyBGMVolumeIgnoreState(bool Value);
	
	UFUNCTION(BlueprintCallable)
	void ApplySFXVolumeIgnoreState(bool Value);
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UCheckBox> MasterSoundVolumeIgnoreCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UCheckBox> BGMSoundVolumeIgnoreCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UCheckBox> SFXSoundVolumeIgnoreCheckBox;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UFMODBus> MasterBus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UFMODBus> AmbienceBus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UFMODBus> BackgroundBus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UFMODBus> SoundEffectBus;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<UFMODBus> InterfaceBus;
};
