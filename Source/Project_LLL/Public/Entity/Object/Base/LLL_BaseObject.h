// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DataAsset/LLL_BaseObjectDataAsset.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_FModInterface.h"
#include "Interface/LLL_NiagaraInterface.h"
#include "LLL_BaseObject.generated.h"

class UNiagaraComponent;
class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_BaseObject : public AActor, public IAbilitySystemInterface, public ILLL_FModInterface, public ILLL_NiagaraInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_BaseObject();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }
	FORCEINLINE virtual TArray<UNiagaraComponent*> GetNiagaraComponents() const override { return NiagaraComponents; }
	
protected:
	// Called when the game starts or when spawned
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;

protected:
	virtual void SetFModParameter(EFModParameter FModParameter) override {}
	virtual void AddNiagaraComponent(UNiagaraComponent* InNiagaraComponent) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraComponent>> NiagaraComponents;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_BaseObjectDataAsset> BaseObjectDataAsset;
};
