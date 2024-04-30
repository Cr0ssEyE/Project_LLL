// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LLL_ShoppingMapComponent.generated.h"

class ALLL_RewardObject;
struct FTestShopDataTable;
class ULLL_ShopDataTable;

UENUM()
enum class EProductType
{
	Gold = 1,
	MaxHP,
	Ability
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LLL_API ULLL_ShoppingMapComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLL_ShoppingMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void DeleteProducts();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetProducts();
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditAnywhere, Category = Product, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ALLL_RewardObject>> ProductList;

	UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestShopDataTable> ShopData;
};
