// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LLL_ShoppingMapComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnShopingDelegate);

class ALLL_RewardObject;
//class ULLL_TestShopDataTable;
//struct FTestShopDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LLL_API ULLL_ShoppingMapComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULLL_ShoppingMapComponent();

	UFUNCTION()
	void DeleteProducts();

	FOnShopingDelegate ShopingDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetProducts();
	
	virtual void BeginDestroy() override;

	UFUNCTION()
	void SetDelegate();
protected:
	UPROPERTY(EditAnywhere, Category = Product, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ALLL_RewardObject>> ProductList;

	/*UPROPERTY(VisibleDefaultsOnly)
	TArray<FTestShopDataTable> ShopData;*/
};
