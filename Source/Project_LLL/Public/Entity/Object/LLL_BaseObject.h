﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DataAsset/LLL_BaseObjectDataAsset.h"
#include "GameFramework/Actor.h"
#include "LLL_BaseObject.generated.h"

class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_BaseObject : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_BaseObject();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE UFMODAudioComponent* GetFModAudioComponent() const { return FModAudioComponent; }
	
protected:
	// Called when the game starts or when spawned
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<const ULLL_BaseObjectDataAsset> BaseObjectDataAsset;
};
