// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "LLL_TitleScreenWidget.generated.h"

class UImage;
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
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayNewGameStartHoverAnimation() { PlayAnimationForward(NewGameButtonHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayNewGameStartUnHoverAnimation() { PlayAnimationReverse(NewGameButtonHover); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayLoadGameHoverAnimation() { PlayAnimationForward(LoadGameButtonHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayLoadGameUnHoverAnimation() { PlayAnimationReverse(LoadGameButtonHover); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlaySettingHoverAnimation() { PlayAnimationForward(SettingButtonHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlaySettingUnHoverAnimation() { PlayAnimationReverse(SettingButtonHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayExitGameHoverAnimation() { PlayAnimationForward(ExitButtonHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayExitGameUnHoverAnimation() { PlayAnimationReverse(ExitButtonHover); }
	
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> NewGameButtonHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> LoadGameButtonHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SettingButtonHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ExitButtonHover;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> LobbyUIPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI, meta = (BindWidget))
	TObjectPtr<UVerticalBox> LobbyHoverVerticalBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UImage> HoveImages;
	
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
