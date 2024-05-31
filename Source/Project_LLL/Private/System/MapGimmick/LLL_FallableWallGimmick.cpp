// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_FallableWallGimmick.h"

#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"


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
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("HitNormal 값 테스트 : %f, %f, %f"), HitNormal.X, HitNormal.Y, HitNormal.Z));
	
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Other);
	if (!IsValid(Monster))
	{
		return;
	}

	if(!Monster->GetAbilitySystemComponent()->GetGameplayTagCount(TAG_GAS_MONSTER_FALLABLE))
	{
		return;
	}

	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER_FALLABLE);
	
	// 여기에 연출 입력
	FallOutEvent();
}

void ALLL_FallableWallGimmick::FallOutEvent()
{
	GetWorldSettings()->SetTimeDilation(0.2f);
	
	FTimerHandle DilationTimerHandle;
	GetWorldTimerManager().SetTimer(DilationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]
	{
		GetWorldSettings()->SetTimeDilation(1.f);
	}), GetWorldSettings()->TimeDilation, false);
}

