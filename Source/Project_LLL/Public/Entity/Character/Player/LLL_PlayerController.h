// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LLL_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API ALLL_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALLL_PlayerController();

	virtual void BeginPlay() override;

	void SetGameInputMode();
	void SetUIInputMode(const TSharedPtr<SWidget>& FocusWidget = nullptr);
};
