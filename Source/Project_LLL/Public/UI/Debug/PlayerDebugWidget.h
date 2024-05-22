// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDebugWidget.h"
#include "PlayerDebugWidget.generated.h"

class UGameplayEffect;
class UButton;
class UCheckBox;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API UPlayerDebugWidget : public UCharacterDebugWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void PlayerMovementCheckBoxEvent(bool value);

	UFUNCTION(BlueprintCallable)
	void PlayerDashCheckBoxEvent(bool value);

	UFUNCTION(BlueprintCallable)
	void PlayerChaseActionCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void PlayerSkillCheckBoxEvent(bool value);

	virtual void CharacterHitCheckCheckBoxEvent(bool value) override;
	virtual void CharacterAttackCheckBoxEvent(bool value) override;
	virtual void CharacterCollisionCheckBoxEvent(bool value) override;

	UFUNCTION(BlueprintCallable)
	void PlayerFillHealthButtonEvent();

	UFUNCTION(BlueprintCallable)
	void PlayerCoolDownResetButtonEvent();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerMovementCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerDashCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerSkillCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> PlayerWireActionCheckBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> PlayerFillHealthButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> PlayerCoolDownResetButton;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS", DisplayName = "플레이어 체력 회복 이펙트")
	TSubclassOf<UGameplayEffect> FillHealthEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GAS", DisplayName = "플레이어 쿨다운 초기화 이펙트")
	TSubclassOf<UGameplayEffect> ResetCoolDownEffect;
};
