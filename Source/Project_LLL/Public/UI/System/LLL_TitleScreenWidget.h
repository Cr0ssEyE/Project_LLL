// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LLL_TitleScreenWidget.generated.h"

class UVerticalBox;
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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
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
	void DisableWidgetActivation();
	
	UFUNCTION(BlueprintCallable)
	void OpenIntroLevel();

	UFUNCTION(BlueprintCallable)
	void OpenSavedLevel();
	
	UFUNCTION()
	void CloseGame();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LobbyUIPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UVerticalBox> LobbyButtonVerticalBox;
	
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
	UPROPERTY(EditDefaultsOnly)
	uint8 bTestNoneSaveFileUI : 1;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsLoadTestLevel : 1;
};
