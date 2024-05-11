// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LLL_TitleScreenWidget.generated.h"

class ULLL_SettingWidget;
class UCanvasPanel;
class UButton;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_TitleScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION()
	void NewGameStartButtonEvent();

	UFUNCTION()
	void LoadGameButtonEvent();
	
	UFUNCTION()
	void SettingButtonEvent();

	UFUNCTION()
	void ExitGameButtonEvent();

	UFUNCTION()
	void ExitConfirmButtonEvent();

	UFUNCTION()
	void ExitCancelButtonEvent();


protected:
	UFUNCTION()
	FORCEINLINE void SetLobbyUIActivation(bool Value) { LobbyUIPanel->SetIsEnabled(Value); }
	
	UFUNCTION()
	void PlayShowLobbyUIAnimation();

	UFUNCTION()
	void OpenIntroLevel();
	
	UFUNCTION()
	void CloseGame();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LobbyUIPanel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> NewGameStartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> LoadGameButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> SettingButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitGameButton;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ExitGameCheckPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitConfirmButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitCancelButton;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<ULLL_SettingWidget> SettingWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LobbyIntroAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LobbyFadeAnimation;

};
