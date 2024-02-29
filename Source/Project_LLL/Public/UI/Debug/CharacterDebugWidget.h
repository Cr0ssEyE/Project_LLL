// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterDebugWidget.generated.h"

class UCheckBox;
/**
 * 
 */
UCLASS()
class PROJECT_LLL_API UCharacterDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	virtual void CharacterHitCheckCheckBoxEvent(bool value) {}

	UFUNCTION(BlueprintCallable)
	virtual void CharacterAttackCheckBoxEvent(bool value) {}

	UFUNCTION(BlueprintCallable)
	virtual void CharacterCollisionCheckBoxEvent(bool value) {}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> CharacterHitCheckCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> CharacterAttackCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> CharacterCollisionCheckBox;
};
