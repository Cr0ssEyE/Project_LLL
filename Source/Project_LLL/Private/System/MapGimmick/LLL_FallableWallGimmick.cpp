// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_FallableWallGimmick.h"

#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/LLL_ShareableNiagaraDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLL_MathHelper.h"


// Sets default values
ALLL_FallableWallGimmick::ALLL_FallableWallGimmick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FallableWall"));
	SetRootComponent(Wall);

	NiagaraComponent->SetupAttachment(Wall);
	FModAudioComponent->SetupAttachment(Wall);
}

// Called when the game starts or when spawned
void ALLL_FallableWallGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALLL_FallableWallGimmick::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if (!IsValid(Monster) || Monster->CheckCharacterIsDead())
	{
		return;
	}

	if(!Monster->GetAbilitySystemComponent()->GetGameplayTagCount(TAG_GAS_MONSTER_FALLABLE))
	{
		return;
	}
	
	FVector OverlapDirection = Monster->GetCharacterMovement()->Velocity.GetSafeNormal2D();

	if (!CheckFallable(OverlapDirection, Monster->GetActorLocation()))
	{
		return;
	}
	
	Monster->GetAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE, 99);
	Monster->CustomTimeDilation = 1.f;
	Monster->GetCharacterMovement()->Velocity = FVector::Zero();
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	
	// 여기에 연출 입력
	FallOutBegin(Monster, OverlapDirection, Monster->GetActorLocation());
}

bool ALLL_FallableWallGimmick::CheckFallable(FVector HitNormal, FVector HitLocation)
{
	FHitResult Result;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel
	(Result,
	HitLocation,
	HitLocation + HitNormal * 10000.f,
	ECC_WALL_ONLY,
	Params
	);

	if (Result.GetActor())
	{
		return false;
	}
	return true;
}

void ALLL_FallableWallGimmick::FallOutBegin(AActor* Actor, FVector HitNormal, FVector HitLocation)
{
	GetWorldSettings()->SetTimeDilation(0.1f);
	CustomTimeDilation = 1.f / GetWorldSettings()->TimeDilation;
	UNiagaraSystem* WallCrashNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetShareableNiagaraDataAsset()->InvisibleWallCrashNiagaraSystem;
	NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallCrashNiagaraSystem, HitLocation, HitNormal.Rotation());
	NiagaraComponent->SetCustomTimeDilation(CustomTimeDilation);
	
	FTimerHandle DilationTimerHandle;
	GetWorldTimerManager().SetTimer(DilationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		GetWorldSettings()->SetTimeDilation(1.f);
		CustomTimeDilation = 1.f;
		FallOutStart(Actor, HitNormal);
	}), GetWorldSettings()->TimeDilation, false);
}

void ALLL_FallableWallGimmick::FallOutStart(AActor* Actor, FVector HitNormal)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Actor);
	CustomTimeDilation = 1.f;
	Monster->CustomTimeDilation = 1.f;
	
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, HitNormal, Monster]
	{
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(HitNormal, Monster->GetKnockBackedPower() * 3.f);
		Monster->LaunchCharacter(LaunchVelocity, true, true);
	}));
}

