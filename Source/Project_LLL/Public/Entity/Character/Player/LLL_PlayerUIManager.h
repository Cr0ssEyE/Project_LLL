// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Entity/Character/Base/LLL_BaseCharacterUIManager.h"
#include "LLL_PlayerUIManager.generated.h"

class ULLL_MainEruriaInfoWidget;
class ULLL_SkillWidget;
class ALLL_InteractiveObject;
class ULLL_InteractionWidget;
class ULLL_InventoryWidget;
class ULLL_PlayerStatusWidget;
class ULLL_GamePauseWidget;
class ULLL_SelectRewardWidget;
class ULLL_PlayerChaseActionWidget;
class ULLL_PlayerComboWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_PlayerUIManager : public ULLL_BaseCharacterUIManager
{
	GENERATED_BODY()

public:	
	ULLL_PlayerUIManager();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:	
	void TogglePauseWidget(bool IsDead) const;
	void ToggleInventoryWidget() const;
	void EnableInteractionWidget() const;
	void DisableInteractionWidget() const;
	void UpdateInteractionWidget(const ALLL_InteractiveObject* CurrentObject, int Num) const;

	void SetAllWidgetVisibility(const bool Visible) const;

	FORCEINLINE ULLL_GamePauseWidget* GetGamePauseWidget() const { return GamePauseWidget; }
	FORCEINLINE ULLL_InventoryWidget* GetInventoryWidget() const { return InventoryWidget; }
	FORCEINLINE ULLL_InteractionWidget* GetInteractionWidget() const { return InteractionWidget; }
	FORCEINLINE ULLL_SelectRewardWidget* GetSelectRewardWidget() const { return SelectRewardWidget; }
	FORCEINLINE ULLL_PlayerChaseActionWidget* GetChaseActionWidget() const { return ChaseActionWidget; }
	FORCEINLINE ULLL_PlayerComboWidget* GetComboWidget() const { return ComboWidget; }
	FORCEINLINE ULLL_MainEruriaInfoWidget* GetMainEruriaWidget() const { return MainEruriaInfoWidget; }
	
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
	TSubclassOf<ULLL_SelectRewardWidget> SelectRewardWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_SelectRewardWidget> SelectRewardWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_PlayerComboWidget> ComboWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerComboWidget> ComboWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_PlayerChaseActionWidget> ChaseActionWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerChaseActionWidget> ChaseActionWidget;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_MainEruriaInfoWidget> MainEruriaInfoWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_MainEruriaInfoWidget> MainEruriaInfoWidget;
};
