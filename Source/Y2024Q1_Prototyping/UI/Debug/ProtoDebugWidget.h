// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterDebugWidget.h"
#include "ObjectDebugWidget.h"
#include "PlayerDebugWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "ProtoDebugWidget.generated.h"

/**
 * 
 */
UENUM()
enum class EDebugWidgetTypes
{
	Player,
	Monster,
	Object
};

UCLASS()
class Y2024Q1_PROTOTYPING_API UProtoDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
protected:
	UFUNCTION(BlueprintCallable)
	void EnablePlayerDebugWidget();

	UFUNCTION(BlueprintCallable)
	void EnableMonsterDebugWidget();

	UFUNCTION(BlueprintCallable)
	void EnableObjectDebugWidget();
	
	UFUNCTION(BlueprintCallable)
	void ToggleWidget();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> DebugWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UPlayerDebugWidget> PlayerDebugWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UMonsterDebugWidget> MonsterDebugWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UObjectDebugWidget> ObjectDebugWidget;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> PlayerDebugButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> MonsterDebugButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ObjectDebugButton;
	
};
