// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterDebugWidget.h"
#include "MonsterDebugWidget.generated.h"

class UCheckBox;

/**
 * 
 */
UCLASS()
class PROJECT_LLL_API UMonsterDebugWidget : public UCharacterDebugWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	void MonsterSpawnDataCheckBoxEvent(bool value);
	
	UFUNCTION(BlueprintCallable)
	void MonsterToggleAICheckBoxEvent(bool value);

	virtual void CharacterHitCheckCheckBoxEvent(bool value) override;
	virtual void CharacterAttackCheckBoxEvent(bool value) override;
	virtual void CharacterCollisionCheckBoxEvent(bool value) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterSpawnDataCheckBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCheckBox> MonsterDisableAICheckBox;
};
