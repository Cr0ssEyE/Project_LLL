// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_FallableWallGimmick.h"

#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"


// Sets default values
ALLL_FallableWallGimmick::ALLL_FallableWallGimmick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FallableWall"));
	SetRootComponent(Wall);
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
	if (!IsValid(Monster))
	{
		return;
	}

	if(!Monster->GetAbilitySystemComponent()->GetGameplayTagCount(TAG_GAS_MONSTER_FALLABLE))
	{
		return;
	}

	// 여기에 연출 입력
}
