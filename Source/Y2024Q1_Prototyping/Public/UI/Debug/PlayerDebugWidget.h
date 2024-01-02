// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDebugWidget.generated.h"

class UButton;
class UCheckBox;
/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API UPlayerDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void PlayerMovementCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerEvadeCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerSkillCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerHitCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerAttackCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerCollisionCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerFillHealthButtonEvent();
	
	UFUNCTION(BlueprintCallable)
	void PlayerCoolDownResetButtonEvent();
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerMovementCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerEvadeCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerSkillCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerHitCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerAttackCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerCollisionCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> PlayerFillHealthButton;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> PlayerCoolDownResetButton;
	
};
