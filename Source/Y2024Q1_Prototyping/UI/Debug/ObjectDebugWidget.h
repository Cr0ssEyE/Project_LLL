// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "ObjectDebugWidget.generated.h"

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API UObjectDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void ObjectGenerateCheckBoxEvent(bool value);
	UFUNCTION(BlueprintCallable)
	void ObjectActivateCheckBoxEvent(bool value);
	UFUNCTION(BlueprintCallable)
	void ObjectDestroyCheckBoxEvent(bool value);
	UFUNCTION(BlueprintCallable)
	void ObjectHitCheckBoxEvent(bool value);
	UFUNCTION(BlueprintCallable)
	void ObjectCollisionCheckBoxEvent(bool value);
	UFUNCTION(BlueprintCallable)
	void ObjectTrapActivateButtonEvent();
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> ObjectGenerateCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> ObjectActivateCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> ObjectDestroyCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> ObjectHitCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> ObjectCollisionCheckBox;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ObjectTrapActivateButton;
	
};
