// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "LLL_PlayerRewardInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ULLL_PlayerRewardInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FORCEINLINE UImage* GetFirstImage() { return RewardImage1; }
	FORCEINLINE UImage* GetSecondImage() { return RewardImage2; }
	FORCEINLINE UImage* GetThirdImage() { return RewardImage3; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardImage1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardImage2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, meta=(BindWidget))
	TObjectPtr<UImage> RewardImage3;
};
