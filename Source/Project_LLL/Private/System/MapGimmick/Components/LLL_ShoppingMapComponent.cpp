// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/Components/LLL_ShoppingMapComponent.h"

#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Entity/Object/Interactive/LLL_EnhanceRewardObject.h"
#include "Entity/Object/Interactive/LLL_MaxHPRewardObject.h"
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Game/LLL_GameInstance.h"
#include "System/MapGimmick/Components/LLL_ProductSpawnPointComponent.h"

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
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	TArray<const FRewardDataTable*> RewardData = GameInstance->GetRewardDataTable();
	for (USceneComponent* ChildComponent : GetOwner()->GetRootComponent()->GetAttachChildren())
	{
		ULLL_ProductSpawnPointComponent* SpawnPoint = Cast<ULLL_ProductSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			ALLL_RewardObject* Product = GetWorld()->SpawnActor<ALLL_RewardObject>(ALLL_RewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			
			/*switch (static_cast<ERewardCategory>(FMath::RandRange(2, 4)))
			{
			case ERewardCategory::Ability:
				Product = GetWorld()->SpawnActor<ALLL_AbilityRewardObject>(ALLL_AbilityRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			case ERewardCategory::Enhance:
				Product = GetWorld()->SpawnActor<ALLL_EnhanceRewardObject>(ALLL_EnhanceRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			case ERewardCategory::MaxHP:
				Product = GetWorld()->SpawnActor<ALLL_MaxHPRewardObject>(ALLL_MaxHPRewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
				break;
			default: ;
			}*/
			const FRewardDataTable* data = RewardData[FMath::RandRange(0, RewardData.Num() - 1)];
			Product->InteractionDelegate.AddUObject(this, & ULLL_ShoppingMapComponent::SetDelegate);
			Product->SetInformation(data);
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

void ULLL_ShoppingMapComponent::SetDelegate()
{
	ShopingDelegate.Broadcast();
}

