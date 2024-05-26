// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "LLL_SelectRewardWidget.generated.h"

class UImage;
class URichTextBlock;
struct FAbilityDataTable;
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
	FORCEINLINE UButton* GetFirstButton() const { return RewardButton1; }
	FORCEINLINE UButton* GetSecondButton() const { return RewardButton2; }
	FORCEINLINE UButton* GetThirdButton() const { return RewardButton3; }

public:
	void SetWidgetInfo(TArray<FAbilityDataTable*> AbilityDataArray);
	
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardOneHoverAnimation() { PlayAnimationForward(RewardOneHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardOneUnHoverAnimation() { PlayAnimationReverse(RewardOneHover); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardTwoHoverAnimation() { PlayAnimationForward(RewardTwoHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardTwoUnHoverAnimation() { PlayAnimationReverse(RewardTwoHover); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardThreeHoverAnimation() { PlayAnimationForward(RewardThreeHover); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardThreeUnHoverAnimation() { PlayAnimationReverse(RewardThreeHover); }

protected:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardOneSelectAnimation() { PlayAnimationForward(RewardOneSelect, 1.f, true); }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardTwoSelectAnimation() { PlayAnimationForward(RewardTwoSelect, 1.f, true); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void PlayRewardThreeSelectAnimation() { PlayAnimationForward(RewardThreeSelect, 1.f, true); }
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardOneHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardTwoHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardThreeHover;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardOneSelect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardTwoSelect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> RewardThreeSelect;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardNameText1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardInfoText1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardBackgroundImage1;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardNameText2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardInfoText2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardBackgroundImage2;

protected:
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UButton> RewardButton3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardNameText3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<URichTextBlock> RewardInfoText3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardBackgroundImage3;

protected:
};
