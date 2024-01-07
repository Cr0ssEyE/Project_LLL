// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLL_PlayerUIManager.generated.h"


class ALLL_InteractiveObject;
class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_PlayerStatusWidget;
class ULLL_GamePauseWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class Y2024Q1_PROTOTYPING_API ULLL_PlayerUIManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLL_PlayerUIManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void TogglePauseWidget() const;
	void ToggleInventoryWidget() const;
	void EnableInteractionWidget() const;
	void DisableInteractionWidget() const;
	void UpdateInteractionWidget(ALLL_InteractiveObject* CurrentObject, int Num) const;
	void UpdateStatusWidget() const;

	UFUNCTION(BlueprintCallable)
	void SetAllWidgetVisibility(const bool Visible);
	
protected:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_GamePauseWidget> PauseWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_GamePauseWidget> PauseWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_InventoryWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_InventoryWidget> InventoryWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_InteractionWidget> InteractionWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_InteractionWidget> InteractionWidget;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_PlayerStatusWidget> PlayerStatusWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerStatusWidget> PlayerStatusWidget;
};
