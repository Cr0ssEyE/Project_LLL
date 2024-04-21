// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "LLL_SelectRewardWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_SelectRewardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE UButton* GetFirstButton() { return RewardButton1; }
	FORCEINLINE UButton* GetSecondButton() { return RewardButton2; }
	FORCEINLINE UButton* GetThirdButton() { return RewardButton3; }

	UFUNCTION(BlueprintCallable)
	void CheckButton();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton3;
};
