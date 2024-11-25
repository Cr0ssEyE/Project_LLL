// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_ThrownFeather.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_ThrownFeatherDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/Object/Thrown/LLL_ThrownFeatherAttributeSet.h"
#include "Util/LLL_AbilityDataHelper.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_ThrownFeather::ALLL_ThrownFeather()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_ThrownFeatherDataAsset>(PATH_THROWN_FEATHER_DATA, EAssertionLevel::Check);
	
	ThrownFeatherAttributeSet = CreateDefaultSubobject<ULLL_ThrownFeatherAttributeSet>(TEXT("PlayerThrownFeatherAttributeSet"));
	
	HitCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit Collision"));
	HitCollisionBox->SetCollisionProfileName(CP_PLAYER_THROWN_OBJECT);
	SetRootComponent(HitCollisionBox);
}

void ALLL_ThrownFeather::BeginPlay()
{
	Super::BeginPlay();

	ThrownFeatherDataAsset = Cast<ULLL_ThrownFeatherDataAsset>(ThrownObjectDataAsset);
	ThrownObjectAttributeSet = ThrownFeatherAttributeSet;

	HitCollisionBox->SetBoxExtent(ThrownFeatherDataAsset->HitCollisionSize);

	ChaseFeatherThrowAngleOffset = ThrownFeatherDataAsset->ChaseFeatherThrowAngleOffset;
	ChaseFeatherThrowAngleRandomMultiply = ThrownFeatherDataAsset->ChaseFeatherThrowAngleRandomMultiply;
}

void ALLL_ThrownFeather::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug())
		{
			HitCollisionBox->SetHiddenInGame(false);
		}
		else
		{
			HitCollisionBox->SetHiddenInGame(true);
		}
	}
#endif
}

void ALLL_ThrownFeather::Activate()
{
	Super::Activate();

	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProjectileMovementComponent->UpdatedComponent = HitCollisionBox;
}

void ALLL_ThrownFeather::Deactivate()
{
	Super::Deactivate();
	
	HitCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALLL_ThrownFeather::Throw(AActor* NewOwner, AActor* NewTarget, float InSpeed, bool Straight, float InKnockBackPower)
{
	Super::Throw(NewOwner, NewTarget, InSpeed, Straight, InKnockBackPower);

	if (const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner()))
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		OffencePower = AbilityData->AbilityValue2 / static_cast<uint32>(AbilityData->Value2Type);
		OffencePower *= PlayerAttributeSet->GetFeatherOffencePowerRate();
		OffencePower = FLLL_AbilityDataHelper::CalculateOffencePower(OffencePower, Player);
		OffencePower += PlayerAttributeSet->GetFeatherOffencePowerPlus();
	}
}

void ALLL_ThrownFeather::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner());
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(Other);
	if (IsValid(Player) && AbilitySystemInterface)
	{
		FVector Direction = (Other->GetActorLocation() - Player->GetActorLocation()).GetSafeNormal2D();
		
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();

		// 맹렬한 공세 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_QUADRUPLE_HIT))
		{
			const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
			const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
			
			float HitCount = 4;
			float HitOffsetTime = 0.01f;
			for (int i = 0; i < HitCount; i++)
			{
				FTimerHandle QuadrupleHitTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(QuadrupleHitTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]{
					FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
					EffectContextHandle.AddSourceObject(this);
					EffectContextHandle.AddInstigator(Player, this);
					const FGameplayEffectSpecHandle EffectSpecHandle1 = ASC->MakeOutgoingSpec(ThrownObjectDataAsset->DamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
					const FGameplayEffectSpecHandle EffectSpecHandle2 = ASC->MakeOutgoingSpec(PlayerDataAsset->RecoveryManaByFeatherRecoveryManaEffect, Player->GetAbilityLevel(), EffectContextHandle);
					if (EffectSpecHandle1.IsValid() && EffectSpecHandle2.IsValid())
					{
						UE_LOG(LogTemp, Log, TEXT("%s에게 %f만큼 데미지 : %d"), *Other->GetName(), OffencePower, i + 2)
						EffectSpecHandle1.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
						EffectSpecHandle2.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, PlayerAttributeSet->GetFeatherManaRecoveryValue());
						
						if (i == 0)
						{
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PlayerDataAsset->QuadrupleHitParticle, Other->GetActorLocation(), FRotator::ZeroRotator, FVector::OneVector, true);
						}
						else
						{
							ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle1, AbilitySystemInterface->GetAbilitySystemComponent());
							ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle2, PlayerASC);
						}
						
						if (i == HitCount - 1)
						{
							KnockBackPower = Player->GetQuadrupleHitKnockBackPower();
							KnockBackPower = FLLL_AbilityDataHelper::CalculateKnockBackPower(KnockBackPower, Player);
							
							KnockBackTo(Direction, Other);
						}
					}
				}), HitOffsetTime, false);

				HitOffsetTime += PlayerDataAsset->QuadrupleHitHitOffsetTime;
			}
		}
	}
}

void ALLL_ThrownFeather::DamageTo(AActor* OtherActor)
{
	Super::DamageTo(OtherActor);

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(GetOwner());
	const IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (IsValid(Player) && AbilitySystemInterface && OtherActor != GetOwner())
	{
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
			
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(GetOwner(), this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->RecoveryManaByFeatherRecoveryManaEffect, AbilityLevel, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_1, PlayerAttributeSet->GetFeatherManaRecoveryValue());
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, PlayerASC);
		}
	}
}
