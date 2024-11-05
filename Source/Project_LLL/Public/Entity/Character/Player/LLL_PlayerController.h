// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LLL_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerInitializedDelegate);

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
	virtual void OnPossess(APawn* InPawn) override;
	
	void SetGameInputMode();
	void SetUIInputMode(const TSharedPtr<SWidget>& FocusWidget = nullptr);

public:
	FORCEINLINE void SetCharacterInitialized() { bIsCharacterInitialized = true; }
	FORCEINLINE void SetWidgetInitialized() { bIsWidgetInitialized = true; }
	FORCEINLINE bool CheckPlayerInitialized() const { return bIsCharacterInitialized && bIsWidgetInitialized; }

public:
	FOnPlayerInitializedDelegate PlayerInitializedDelegate;
	
protected:
	void WaitPlayerCharacterInitialize();

protected:
	uint8 bIsCharacterInitialized : 1;
	uint8 bIsWidgetInitialized : 1;
};
