// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLL_BaseCharacterUIManager.generated.h"


class ULLL_CharacterWidgetComponent;
class ULLL_CharacterStatusWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_LLL_API ULLL_BaseCharacterUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULLL_BaseCharacterUIManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE ULLL_CharacterStatusWidget* GetCharacterStatusWidget() const { return CharacterStatusWidget; }
	
protected:
	UFUNCTION()
	void UpdateStatusWidget();

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_CharacterStatusWidget> CharacterStatusWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_CharacterStatusWidget> CharacterStatusWidget;
};
