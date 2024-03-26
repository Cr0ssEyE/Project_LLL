// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacterUIManager.h"
#include "LLL_PlayerUIManager.generated.h"

class ULLL_SkillWidget;
class ALLL_InteractiveObject;
class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_PlayerStatusWidget;
class ULLL_GamePauseWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_PlayerUIManager : public ULLL_BaseCharacterUIManager
{
	GENERATED_BODY()

public:	
	ULLL_PlayerUIManager();

protected:
	virtual void BeginPlay() override;

public:	
	void TogglePauseWidget(bool IsDead) const;
	void ToggleInventoryWidget() const;
	void EnableInteractionWidget() const;
	void DisableInteractionWidget() const;
	void UpdateInteractionWidget(ALLL_InteractiveObject* CurrentObject, int Num) const;

	void SetAllWidgetVisibility(const bool Visible);

	FORCEINLINE ULLL_GamePauseWidget* GetGamePauseWidget() const { return GamePauseWidget; }
	FORCEINLINE ULLL_InventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE ULLL_InteractionWidget* GetInteractionWidget() const { return InteractionWidget; }

protected:
	virtual void UpdateWidget() override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_GamePauseWidget> GamePauseWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_GamePauseWidget> GamePauseWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_InventoryWidget> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_InventoryWidget> InventoryWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_InteractionWidget> InteractionWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_InteractionWidget> InteractionWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_SkillWidget> SkillGaugeWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_SkillWidget> SkillGaugeWidget;
};
