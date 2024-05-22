// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "DataAsset/LLL_BaseObjectDataAsset.h"
#include "GameFramework/Actor.h"
#include "Interface/LLL_FModInterface.h"
#include "LLL_BaseObject.generated.h"

class UNiagaraComponent;
class UFMODAudioComponent;

UCLASS()
class PROJECT_LLL_API ALLL_BaseObject : public AActor, public IAbilitySystemInterface, public ILLL_FModInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALLL_BaseObject();

	FORCEINLINE virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	FORCEINLINE virtual UFMODAudioComponent* GetFModAudioComponent() const override { return FModAudioComponent; }
	FORCEINLINE virtual void SetPitch(float InPitch) override { Pitch = InPitch; }
	
protected:
	// Called when the game starts or when spawned
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void SetDefaultInformation();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InitAttributeSet();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFMODAudioComponent> FModAudioComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<const ULLL_BaseObjectDataAsset> BaseObjectDataAsset;

	UPROPERTY(EditAnywhere)
	float Pitch;
	
	UPROPERTY(VisibleAnywhere)
	float CurrentPitch;
};
