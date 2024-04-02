// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LLL_PlayerGoldComponet.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerMoneyChangedDelegate, float);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LLL_API ULLL_PlayerGoldComponet : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLL_PlayerGoldComponet();

	FOnPlayerMoneyChangedDelegate FOnMoneyChanged;
	FORCEINLINE void IncreaseMoney(const float InMoney) { Money += InMoney; FOnMoneyChanged.Broadcast(GetMoney()); ShowWidget(); }
	FORCEINLINE void DecreaseMoney(const float OutMoney) { Money = Money - OutMoney >= 0 ? Money - OutMoney : 0; FOnMoneyChanged.Broadcast(GetMoney()); ShowWidget();}
	FORCEINLINE const float GetMoney() { return Money; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	float Money;

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class ULLL_PlayerGoldWidget> GoldWidgetClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ULLL_PlayerGoldWidget> GoldWidget;

	FTimerHandle WidgetWaitHideTimerHandle;

	UPROPERTY(VisibleAnywhere)
	uint8 WidgetHideWaitTime;

	UPROPERTY()
	bool IsShowWidget;

	void EnableInteractionWidget();
	void DisableInteractionWidget();
	void ShowWidget(); 
};
