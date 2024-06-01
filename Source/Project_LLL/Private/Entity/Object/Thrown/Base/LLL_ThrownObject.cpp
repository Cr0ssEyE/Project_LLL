// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"

#include "AbilitySystemComponent.h"
#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"

ALLL_ThrownObject::ALLL_ThrownObject()
{
	if (IsValid(BaseMesh))
	{
		SetRootComponent(BaseMesh);
	}
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	if (IsValid(ProjectileMovementComponent))
	{
		ProjectileMovementComponent->bShouldBounce = false;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		ProjectileMovementComponent->InitialSpeed = 0.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->Deactivate();
	}
}

void ALLL_ThrownObject::BeginPlay()
{
	Super::BeginPlay();

	ThrownObjectDataAsset = Cast<ULLL_ThrownObjectDataAsset>(BaseObjectDataAsset);

	FModAudioComponent->SetEvent(ThrownObjectDataAsset->FModEvent);
}

void ALLL_ThrownObject::Activate()
{
	bIsActivated = true;

	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->Activate();
	SetActorHiddenInGame(false);
	UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
}

void ALLL_ThrownObject::Deactivate()
{
	bIsActivated = false;

	FModAudioComponent->Stop();
	FModAudioComponent->Release();
	if (IsValid(NiagaraComponent))
	{
		NiagaraComponent->DestroyComponent();
	}
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
	GetWorldTimerManager().ClearTimer(HideTimerHandle);
}

void ALLL_ThrownObject::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed)
{
	SetOwner(NewOwner);
	Target = NewTarget;

	ProjectileMovementComponent->MaxSpeed = InSpeed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;

	FModAudioComponent->SetPitch(Owner->CustomTimeDilation);
	FModAudioComponent->Play();
	
	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ALLL_ThrownObject::Deactivate, ThrownObjectAttributeSet->GetHideTimer(), false);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwner());
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, OwnerCharacter->GetCharacterLevel(), EffectContextHandle);

	float OffencePower = 0.0f;
	if (Cast<ALLL_MonsterBase>(OwnerCharacter))
	{
		const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(OwnerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
		OffencePower = MonsterAttributeSet->GetOffensePower();
	}
	else if (Cast<ALLL_PlayerBase>(OwnerCharacter))
	{
		OffencePower = (AbilityData->AbilityValue + AbilityData->ChangeValue * AbilityLevel) / static_cast<uint32>(AbilityData->AbilityValueType);
	}
	EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_EFFECT_VALUE, OffencePower);
	
	if(EffectSpecHandle.IsValid())
	{
		if (const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Other))
		{
			UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *Other->GetName())
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
		}
	}
	
	Deactivate();
}
