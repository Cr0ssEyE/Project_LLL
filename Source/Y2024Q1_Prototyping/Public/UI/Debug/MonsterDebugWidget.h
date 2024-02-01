// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterDebugWidget.generated.h"

class UCheckBox;

/**
 * 
 */
UCLASS()
class Y2024Q1_PROTOTYPING_API UMonsterDebugWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void MonsterToggleAICheckBoxEvent(bool value);

	UFUNCTION(BlueprintCallable)
	void MonsterHitCheckCheckBoxEvent(bool value);

	UFUNCTION(BlueprintCallable)
	void MonsterAttackCheckBoxEvent(bool value);

	UFUNCTION(BlueprintCallable)
	void MonsterCollisionCheckBoxEvent(bool value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterAIDisableCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterHitCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterAttackCheckBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterCollisionCheckBox;
};
