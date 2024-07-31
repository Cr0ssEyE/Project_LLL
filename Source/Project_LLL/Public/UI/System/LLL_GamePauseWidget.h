// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_GamePauseWidget.generated.h"

class UTextBlock;
class ULLL_SettingWidget;
class UOverlay;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GamePauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
public:
	void SetupPauseState();
	void RestorePauseState(bool EnableInput = true);
	void SetupDeadStateLayout() const;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ResumeButtonEvent();

	UFUNCTION(BlueprintCallable)
	void SettingButtonEvent();

	UFUNCTION(BlueprintCallable)
	void TitleButtonEvent();
	
	UFUNCTION(BlueprintCallable)
	void ExitButtonEvent();

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayResumeButtonHoverAnimation() { PlayAnimationForward(ResumeHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayResumeButtonUnHoverAnimation() { PlayAnimationReverse(ResumeHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlaySettingButtonHoverAnimation() { PlayAnimationForward(SettingHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlaySettingButtonUnHoverAnimation() { PlayAnimationReverse(SettingHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayTitleButtonHoverAnimation() { PlayAnimationForward(TitleHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayTitleButtonUnHoverAnimation() { PlayAnimationReverse(TitleHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayExitButtonHoverAnimation() { PlayAnimationForward(ExitHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayExitButtonUnHoverAnimation() { PlayAnimationReverse(ExitHover); }
	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ResumeHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> SettingHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TitleHover;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ExitHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ResetAnim;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UTextBlock> PauseTypeText;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> TitleButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<ULLL_SettingWidget> SettingWidget;

	TObjectPtr<UButton> LastClickButton;
};
