// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LLL_DamageWidget.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "LLL_FloatingDamageActor.generated.h"

UCLASS()
class PROJECT_LLL_API ALLL_FloatingDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALLL_FloatingDamageActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent> RootComponent;

	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<ULLL_DamageWidget> DamageWidgetClass;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<ULLL_DamageWidget> DamageWidget;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> DamageWidgetComponent;
};
