// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_GamePauseWidget.generated.h"

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

public:
	void SetupDeadStateLayout() const;
	
protected:
	UFUNCTION(BlueprintCallable)
	void ResumeButtonEvent();

	UFUNCTION(BlueprintCallable)
	void SettingButtonEvent();

	UFUNCTION(BlueprintCallable)
	void ExitButtonEvent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ResumeButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SettingButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ExitButton;
};
