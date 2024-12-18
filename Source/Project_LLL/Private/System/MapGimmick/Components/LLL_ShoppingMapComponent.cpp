// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/Components/LLL_ShoppingMapComponent.h"
#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "Game/LLL_GameInstance.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Game/LLL_RewardGimmickSubsystem.h"
#include "Kismet/GameplayStatics.h"
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
	TMap<int32, int32> SavedShoppingProductList;
	if (IsValid(GameInstance->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()))
	{
		SavedShoppingProductList = GameInstance->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()->PlayerPlayProgressData.ShoppingProductList;
	}
	GameInstance->GetSubsystem<ULLL_RewardGimmickSubsystem>()->SetRewardButtons();

	int32 ProductIndex = 0;
	for (USceneComponent* ChildComponent : GetOwner()->GetRootComponent()->GetAttachChildren())
	{
		ULLL_ProductSpawnPointComponent* SpawnPoint = Cast<ULLL_ProductSpawnPointComponent>(ChildComponent);
		if (IsValid(SpawnPoint))
		{
			/*ALLL_RewardObject* Product = GetWorld()->SpawnActor<ALLL_RewardObject>(ALLL_RewardObject::StaticClass(), SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			FVector Vector = Product->GetActorLocation();
			Vector.Z += 150;
			Product->SetActorLocation(Vector);

			
			Product->InteractionDelegate.AddUObject(this, &ULLL_ShoppingMapComponent::SetDelegate);
			Product->SetInformation(ProductData, Index);
			Product->ApplyProductEvent();
			ProductList.Add(Product);*/
		}
	}
}

void ULLL_ShoppingMapComponent::BeginDestroy()
{
	DeleteProducts();
	Super::BeginDestroy();
}

void ULLL_ShoppingMapComponent::SetDelegate(ALLL_RewardObject* ProductObject)
{
	if (ProductList.Find(ProductObject))
	{
		ProductList.Remove(ProductObject);
	}

	// 상품 구매시 세이브는 생각좀 필요
	// Cast<ULLL_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetSubsystem<ULLL_GameProgressManageSubSystem>()->BeginSaveGame();
	
	ShoppingDelegate.Broadcast();
}

