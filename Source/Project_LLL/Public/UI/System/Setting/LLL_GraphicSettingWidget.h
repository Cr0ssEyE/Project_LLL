// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LLL_GraphicSettingWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_GraphicSettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetButtonStates();
	
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyTextureQualityLow();

	UFUNCTION(BlueprintCallable)
	void ApplyTextureQualityMiddle();

	UFUNCTION(BlueprintCallable)
	void ApplyTextureQualityHigh();
	
	UFUNCTION(BlueprintCallable)
	void ApplyShadowQualityLow();

	UFUNCTION(BlueprintCallable)
	void ApplyShadowQualityMiddle();

	UFUNCTION(BlueprintCallable)
	void ApplyShadowQualityHigh();

	UFUNCTION(BlueprintCallable)
	void ApplyGIQualityLow();

	UFUNCTION(BlueprintCallable)
	void ApplyGIQualityMiddle();
	
	UFUNCTION(BlueprintCallable)
	void ApplyGIQualityHigh();
	
	UFUNCTION(BlueprintCallable)
	void ApplyShadingQualityLow();

	UFUNCTION(BlueprintCallable)
	void ApplyShadingQualityMiddle();
	
	UFUNCTION(BlueprintCallable)
	void ApplyShadingQualityHigh();
	
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyPPQualityLow();

	UFUNCTION(BlueprintCallable)
	void ApplyPPQualityMiddle();

	UFUNCTION(BlueprintCallable)
	void ApplyPPQualityHigh();
	
	UFUNCTION(BlueprintCallable)
	void ApplyAAQualityOff();

	UFUNCTION(BlueprintCallable)
	void ApplyAAQualityMiddle();

	UFUNCTION(BlueprintCallable)
	void ApplyAAQualityHigh();
	
protected:
	UPROPERTY()
	TArray<TObjectPtr<UButton>> TextureOptionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> TextureQualityLowButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> TextureQualityMiddleButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> TextureQualityHighButton;

	UPROPERTY()
	TArray<TObjectPtr<UButton>> ShadowOptionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadowQualityLowButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadowQualityMiddleButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadowQualityHighButton;

	UPROPERTY()
	TArray<TObjectPtr<UButton>> GIOptionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> GIQualityLowButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> GIQualityMiddleButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> GIQualityHighButton;

	UPROPERTY()
	TArray<TObjectPtr<UButton>> ShadingOptionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadingQualityLowButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadingQualityMiddleButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> ShadingQualityHighButton;

protected:
	UPROPERTY()
	TArray<TObjectPtr<UButton>> PPOptionButtons;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> PPQualityLowButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> PPQualityMiddleButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> PPQualityHighButton;
	
	UPROPERTY()
	TArray<TObjectPtr<UButton>> AAOptionButtons;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> AAQualityOffButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> AAQualityMiddleButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (BindWidget))
	TObjectPtr<UButton> AAQualityHighButton;
	
};
