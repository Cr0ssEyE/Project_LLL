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

void ALLL_FallableWallGimmick::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Other);
	if (!IsValid(Monster) || Monster->CheckCharacterIsDead())
	{
		return;
	}

	if(!Monster->GetAbilitySystemComponent()->GetGameplayTagCount(TAG_GAS_MONSTER_FALLABLE))
	{
		return;
	}

	Monster->GetAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE, 99);
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	// 여기에 연출 입력
	FallOutBegin(Monster, HitNormal, HitLocation);
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
	
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(HitNormal, Monster->GetKnockBackedPower()) * 3.f;
	Monster->AddKnockBackVelocity(LaunchVelocity, Monster->GetKnockBackedPower());
}

