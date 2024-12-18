// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProtoDebugWidget.generated.h"

class UButton;
class UObjectDebugWidget;
class UMonsterDebugWidget;
class UPlayerDebugWidget;
class UWidgetSwitcher;

/**
 * 
 */

UCLASS()
class PROJECT_LLL_API UProtoDebugWidget : public UUserWidget
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
	
	UFUNCTION(BlueprintCallable)
	void ToggleSoundMessage();

	UFUNCTION(BlueprintCallable)
	void AllEnuriaDebug();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SoundDebugButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> AllEnuriaDebugButton;

protected:
	UFUNCTION()
	void ReceivePlayerEffectsHandle(TArray<TSoftClassPtr<ULLL_ExtendedGameplayEffect>>& LoadedEffects, int32 EffectID);
};
