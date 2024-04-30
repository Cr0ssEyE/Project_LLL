// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_ShoppingMapComponent.h"

#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Entity/Object/Interactive/LLL_GoldRewardObject.h"
#include "Entity/Object/Interactive/LLL_MaxHPRewardObject.h"
#include "Entity/Object/Interactive/LLL_RewardObject.h"
#include "Game/LLL_GameInstance.h"
#include "System/MapGimmick/LLL_ProductSpawnPointComponent.h"

// Sets default values for this component's properties
ULLL_ShoppingMapComponent::ULLL_ShoppingMapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void ULLL_ShoppingMapComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	ShopData = GameInstance->GetShopDataTable();
}

void ULLL_ShoppingMapComponent::DeleteProducts()
{
	for (auto product : ProductList)
	{
		if (IsValid(product))
		{
			product->Destroy();
		}
	}
	ProductList.Empty();
}


// Called every frame
void ULLL_ShoppingMapComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULLL_ShoppingMapComponent::SetProducts()
{
	for (USceneComponent* ChildComponent : GetOwner()->GetRootComponent()->GetAttachChildren())
	{
		ULLL_ProductSpawnPointComponent* SpawnPoint = Cast<ULLL_ProductSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			ALLL_RewardObject* Product;
			uint8 Index = FMath::RandRange(0, ShopData.Num() - 1);
			switch (ShopData[Index].RewardID)
			{
			case static_cast<int>(EProductType::Gold):
				Product = GetWorld()->SpawnActor<ALLL_GoldRewardObject>(ALLL_GoldRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			case static_cast<int>(EProductType::MaxHP):
				Product = GetWorld()->SpawnActor<ALLL_MaxHPRewardObject>(ALLL_MaxHPRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			case static_cast<int>(EProductType::Ability):
				Product = GetWorld()->SpawnActor<ALLL_AbilityRewardObject>(ALLL_AbilityRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			default: ;
			}
			Product->ApplyProductEvent();
			ProductList.Add(Product);
		}
	}
}

void ULLL_ShoppingMapComponent::BeginDestroy()
{
	DeleteProducts();
	Super::BeginDestroy();
}

