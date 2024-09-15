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
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/Base/LLL_ThrownObjectAttributeSet.h"
#include "Util/LLL_MathHelper.h"

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

void ALLL_ThrownObject::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsActivated() && !bIsStraight)
	{
		if (bTargetIsDead && FVector::Distance(GetActorLocation(), TargetDeadLocation) <= TargetCapsuleRadius)
		{
			Deactivate();
		}

		const FVector TargetLocation = bTargetIsDead ? TargetDeadLocation : Target->GetActorLocation();
		const FVector Direction = TargetLocation - GetActorLocation();
		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), Rotation, DeltaSeconds, CurveSpeed));
		CurveSpeed += 1.0f / CurveSize;
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	}
}

void ALLL_ThrownObject::Activate()
{
	bIsActivated = true;

	AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Log, TEXT("활성화"))
	ProjectileMovementComponent->Activate();
	SetActorHiddenInGame(false);
	AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(BaseObjectDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), FVector::Zero(), FRotator::ZeroRotator, BaseObjectDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));

	CurveSize = ThrownObjectAttributeSet->GetCurveSize();
	CurveSpeed = 1.0f / ThrownObjectAttributeSet->GetCurveSize();
}

void ALLL_ThrownObject::Deactivate()
{
	bIsActivated = false;
	
	FModAudioComponent->Stop();
	FModAudioComponent->Release();
	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
		TempNiagaraComponent->DestroyComponent();
		NiagaraComponents.Remove(TempNiagaraComponent);
	}
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UE_LOG(LogTemp, Log, TEXT("비활성화"))
	UE_LOG(LogTemp, Log, TEXT("%f %f %f"),ProjectileMovementComponent->Velocity.X, ProjectileMovementComponent->Velocity.Y, ProjectileMovementComponent->Velocity.Z)
	ProjectileMovementComponent->Deactivate();
	SetActorHiddenInGame(true);
	
	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter) && TargetCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_ThrownObject::TargetDeadHandle))
	{
		TargetCharacter->CharacterDeadDelegate.RemoveDynamic(this, &ALLL_ThrownObject::TargetDeadHandle);
	}
	bTargetIsDead = false;

	GetWorldTimerManager().ClearTimer(HideTimerHandle);
}

void ALLL_ThrownObject::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	SetOwner(NewOwner);
	Target = NewTarget;

	UE_LOG(LogTemp, Log, TEXT("벨로시티 입력"))
	ProjectileMovementComponent->MaxSpeed = InSpeed;
	ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileMovementComponent->MaxSpeed;
	
	FModAudioComponent->SetPitch(Owner->CustomTimeDilation);
	FModAudioComponent->Play();

	ALLL_BaseCharacter* OwnerCharacter = Cast<ALLL_BaseCharacter>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		if (Cast<ALLL_MonsterBase>(OwnerCharacter))
		{
			const ULLL_MonsterAttributeSet* MonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(OwnerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
			OffencePower = MonsterAttributeSet->GetOffensePower();
		}
		else if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OwnerCharacter))
		{
			const ULLL_PlayerCharacterAttributeSet* PlayerCharacterAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(OwnerCharacter->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
			OffencePower = AbilityData->AbilityValue2 * AbilityLevel / static_cast<uint32>(AbilityData->Value2Type);
			OffencePower += PlayerCharacterAttributeSet->GetOffensePower() - Player->GetOriginOffencePower();

			UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
			if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_QUADRUPLE_HIT))
			{
				OffencePower *= Player->GetQuadrupleHitDamageRate();
			}
		}
	}

	ALLL_BaseCharacter* TargetCharacter = Cast<ALLL_BaseCharacter>(Target);
	if (IsValid(TargetCharacter))
	{
		TargetCapsuleRadius = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

		if (!TargetCharacter->CheckCharacterIsDead() && !TargetCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_ThrownObject::TargetDeadHandle))
		{
			TargetCharacter->CharacterDeadDelegate.AddDynamic(this, &ALLL_ThrownObject::TargetDeadHandle);
		}
		else
		{
			TargetDeadLocation = Target->GetActorLocation();
			bTargetIsDead = true;
		}
	}
	
	bIsStraight = Straight;
	KnockBackPower = InKnockBackPower;

	GetWorldTimerManager().SetTimer(HideTimerHandle, this, &ALLL_ThrownObject::Deactivate, ThrownObjectAttributeSet->GetHideTimer(), false);
}

void ALLL_ThrownObject::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (!bIsStraight && Other != Target)
		return;
		
	const ALLL_BaseCharacter* OwnerCharacter = Cast<ALLL_BaseCharacter>(GetOwner());
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Other);
	if (IsValid(OwnerCharacter) && AbilitySystemInterface)
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, OwnerCharacter->GetCharacterLevel(), EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			if (Cast<ALLL_MonsterBase>(OwnerCharacter))
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_RANGED_MONSTER_SPAWN_THROWN_OBJECT_OFFENCE_POWER, OffencePower);

				UE_LOG(LogTemp, Log, TEXT("%s에게 %f만큼 데미지"), *Other->GetName(), OffencePower)
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
				KnockBackTarget(Other);

				Deactivate();
			}
			else if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(OwnerCharacter))
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_2, OffencePower);
				
				UE_LOG(LogTemp, Log, TEXT("%s에게 %f만큼 데미지 : 1"), *Other->GetName(), OffencePower)
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
				KnockBackTarget(Other);

				UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
				const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
				
				if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_QUADRUPLE_HIT))
				{
					float HitCount = 4;
					const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
					float HitOffsetTime = PlayerDataAsset->QuadrupleHitHitOffsetTime;
					for (int i = 0; i < HitCount - 1; i++)
					{
						FTimerHandle QuadrupleHitTimerHandle;
						GetWorld()->GetTimerManager().SetTimer(QuadrupleHitTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&, i, HitCount, Other, EffectSpecHandle, AbilitySystemInterface, PlayerAttributeSet, Player]{
							UE_LOG(LogTemp, Log, TEXT("%s에게 %f만큼 데미지 : %d"), *Other->GetName(), OffencePower, i + 2)
							ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, AbilitySystemInterface->GetAbilitySystemComponent());
							if (i == HitCount - 2)
							{
								KnockBackPower = Player->GetQuadrupleHitKnockBackPower();
								KnockBackPower += PlayerAttributeSet->GetKnockBackPower() - Player->GetOriginKnockBackPower();
								KnockBackTarget(Other);
								
								Deactivate();
							}
						}), HitOffsetTime, false);

						HitOffsetTime += 0.1f;
					}
				}
			}
		}
	}
}

void ALLL_ThrownObject::KnockBackTarget(AActor* Other) const
{
	ILLL_KnockBackInterface* KnockBackActor = Cast<ILLL_KnockBackInterface>(Other);
	if (KnockBackActor && KnockBackPower != 0.0f)
	{
		const FVector AvatarLocation = GetActorLocation();
		const FVector LaunchDirection = (Other->GetActorLocation() - AvatarLocation).GetSafeNormal2D();
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(LaunchDirection, KnockBackPower);
		UE_LOG(LogTemp, Log, TEXT("넉백 수행(투사체) : %f"), KnockBackPower)
		KnockBackActor->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
	}
}

void ALLL_ThrownObject::TargetDeadHandle(ALLL_BaseCharacter* Character)
{
	TargetDeadLocation = Character->GetActorLocation();
	bTargetIsDead = true;
}
