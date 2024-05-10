// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LLL_TitleScreenWidget.generated.h"

class ULLL_GameSettingWidget;
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
	virtual void NativeConstruct();

protected:
	UFUNCTION()
	void GameStartBtnEvent();

	UFUNCTION()
	void SettingBtnEvent();

	UFUNCTION()
	void ExitGameBtnEvent();

	UFUNCTION()
	void ExitConfirmBtnEvent();

	UFUNCTION()
	void ExitCancelBtnEvent();


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
	TObjectPtr<UButton> GameStartBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> SettingBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ExitGameBtn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ExitGameCheckPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> ConfirmBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UButton> CancelBtn;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<ULLL_GameSettingWidget> SettingWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LobbyIntroAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LobbyFadeAnimation;

};
