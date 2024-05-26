// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Enumeration/LLL_UserInterfaceEnumHelper.h"
#include "LLL_SettingWidget.generated.h"

class ULLL_GameplaySettingWidget;
class ULLL_SoundSettingWidget;
class ULLL_GraphicSettingWidget;
class ULLL_DisplaySettingWidget;
class UWidgetSwitcher;
enum class EResolutionTypes : uint8;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void CloseSettingWidget();

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SwitchDisplaySettingWidget() { SettingWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(ESettingWidgetType::Display)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SwitchGraphicSettingWidget() { SettingWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(ESettingWidgetType::Graphic)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SwitchSoundSettingWidget() { SettingWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(ESettingWidgetType::Sound)); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SwitchGameplaySettingWidget() { SettingWidgetSwitcher->SetActiveWidgetIndex(static_cast<int>(ESettingWidgetType::Gameplay)); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> SettingWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<ULLL_DisplaySettingWidget> DisplaySettingWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<ULLL_GraphicSettingWidget> GraphicSettingWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<ULLL_SoundSettingWidget> SoundSettingWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<ULLL_GameplaySettingWidget> GameplaySettingWidget;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> DisplaySettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> GraphicSettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SoundSettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> GameplaySettingButton;
};
