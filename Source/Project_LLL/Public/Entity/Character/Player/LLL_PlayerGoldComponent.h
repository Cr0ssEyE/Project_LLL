// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLL_PlayerGoldComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerMoneyChangedDelegate, float);

class ULLL_PlayerGoldWidget;
class ULLL_GoldComponentDataAsset;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LLL_API ULLL_PlayerGoldComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLL_PlayerGoldComponent();

	FOnPlayerMoneyChangedDelegate FOnMoneyChanged;
	void IncreaseMoney(const float InMoney);
	void DecreaseMoney(const float OutMoney);
	FORCEINLINE const float GetMoney() { return Money; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float Money;

	UPROPERTY(EditDefaultsOnly)
	float BeforeMoneyData;

	UPROPERTY(EditDefaultsOnly)
	float InitMoney;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<ULLL_PlayerGoldWidget> GoldWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ULLL_PlayerGoldWidget> GoldWidget;

	FTimerHandle WidgetWaitHideTimerHandle;

	FTimerHandle MoneyTextChangeHandle;

	UPROPERTY(VisibleAnywhere)
	float WidgetHideWaitTime;

	UPROPERTY(EditDefaultsOnly)
	uint8 bIsShowWidget : 1;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULLL_GoldComponentDataAsset> GoldComponentDataAsset;

	void PlayShowInitGoldWidgetAnim();
	void PlayHideInitGoldWidgetAnim();
	void SetMoneyData();
	void ShowWidget(); 
};
