// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_DamageWidget.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "LLL_FloatingDamageActor.generated.h"

class UWidgetComponent;
UCLASS()
class PROJECT_LLL_API ALLL_FloatingDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_FloatingDamageActor();

	FORCEINLINE void SetWidgetText(float Damage) const { DamageWidget->SetText(Damage); }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void DestroySelf();
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ULLL_DamageWidget> DamageWidgetClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_DamageWidget> DamageWidget;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> DamageWidgetComponent;
};
