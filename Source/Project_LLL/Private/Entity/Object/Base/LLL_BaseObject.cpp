// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Base/LLL_BaseObject.h"

#include "AbilitySystemComponent.h"
#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Constant/LLL_GeneralConstants.h"

ALLL_BaseObject::ALLL_BaseObject()
{
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(BaseMesh);
	
	FModAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FModAudioComponent"));
	FModAudioComponent->SetupAttachment(RootComponent);
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
		BaseMesh->SetBoundsScale(ACTOR_DEFAULT_BOUNDS);
	}
}

void ALLL_BaseObject::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(BaseObjectDataAsset))
	{
		return;
	}
	
	if (IsValid(ASC))
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
	
	if (IsValid(BaseObjectDataAsset->InitEffect))
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(BaseObjectDataAsset->InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}

	if (IsValid(BaseObjectDataAsset->Particle))
	{
		AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), BaseObjectDataAsset->ParticleLocation, FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));
	}
}

void ALLL_BaseObject::AddNiagaraComponent(UNiagaraComponent* InNiagaraComponent)
{	
	NiagaraComponents.Remove(nullptr);
	NiagaraComponents.Emplace(InNiagaraComponent);
}
