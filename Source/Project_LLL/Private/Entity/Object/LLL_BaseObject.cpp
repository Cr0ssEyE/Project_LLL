// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/LLL_BaseObject.h"

#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ALLL_BaseObject::ALLL_BaseObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	SetRootComponent(BaseMesh);
}

void ALLL_BaseObject::PostLoad()
{
	Super::PostLoad();

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	SetDefaultInformation();
#endif
}

void ALLL_BaseObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	SetDefaultInformation();
#endif
}

void ALLL_BaseObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetDefaultInformation();
}

void ALLL_BaseObject::SetDefaultInformation()
{
	if (IsValid(BaseObjectDataAsset))
	{
		BaseMesh->SetStaticMesh(BaseObjectDataAsset->StaticMesh);
		BaseMesh->SetRelativeScale3D(BaseObjectDataAsset->MeshScale);
	}
}

// Called when the game starts or when spawned
void ALLL_BaseObject::BeginPlay()
{
	Super::BeginPlay();

	if(!IsValid(BaseObjectDataAsset))
	{
		return;
	}
	
	if(IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		for (const auto ActiveAbility : BaseObjectDataAsset->ActiveGameplayAbility)
		{
			if(IsValid(ActiveAbility))
			{
				FGameplayAbilitySpec AbilitySpec(ActiveAbility);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
	
	if(IsValid(BaseObjectDataAsset->InitEffect))
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(BaseObjectDataAsset->InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

// Called every frame
void ALLL_BaseObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALLL_BaseObject::DelayedDestroy(float Time)
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDelegate;
	DestroyTimerDelegate.BindUObject(this, &ALLL_BaseObject::DestroyTimerCallback);
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDelegate, Time, false);
}

void ALLL_BaseObject::DelayedHide(float Time)
{
	FTimerHandle HideTimerHandle;
	FTimerDelegate HideTimerDelegate;
	HideTimerDelegate.BindUObject(this, &ALLL_BaseObject::HideTimerCallback);
	GetWorldTimerManager().SetTimer(HideTimerHandle, HideTimerDelegate, Time, false);
}

void ALLL_BaseObject::DestroyTimerCallback()
{
	Destroy();
}

void ALLL_BaseObject::HideTimerCallback()
{
	SetActorHiddenInGame(true);
}

