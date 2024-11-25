// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_FallableWallGimmick.h"

#include "FMODAudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/LLL_FallOutSubsytem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Util/LLL_MathHelper.h"


// Sets default values
ALLL_FallableWallGimmick::ALLL_FallableWallGimmick()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FallableWall"));
	SetRootComponent(Wall);

	FModAudioComponent->SetupAttachment(Wall);
}

// Called when the game starts or when spawned
void ALLL_FallableWallGimmick::BeginPlay()
{
	Super::BeginPlay();
	Wall->SetCollisionProfileName(CP_INVISIBLE_WALL);
	Wall->SetVisibility(false);
	
	FallRequiredVelocityLength = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->FallRequiredVelocityLength;
}

void ALLL_FallableWallGimmick::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(OtherActor);
	if (!IsValid(Monster) || Monster->CheckCharacterIsDead() || Monster->GetCapsuleComponent()->GetCollisionProfileName() == CP_OVERLAP_ALL)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("유효하지 않은 대상 오버랩 감지 또는 이미 사망 예정인 대상 감지")));
		return;
	}

	if(!Monster->GetAbilitySystemComponent()->GetGameplayTagCount(TAG_GAS_MONSTER_FALLABLE))
	{
		return;
	}

	FVector TargetVelocity = Monster->GetCharacterMovement()->Velocity;
	float TargetVelocityLength = Monster->GetCharacterMovement()->Velocity.Length();
	if(TargetVelocityLength < FallRequiredVelocityLength)
	{
		return;
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterHitCheckDebug() || DebugGameInstance->CheckMonsterCollisionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("오버랩 액터 속도 감지 : %f, %f, %f || %f"), TargetVelocity.X, TargetVelocity.Y, TargetVelocity.Z, TargetVelocityLength));
		}
	}
#endif
	
	FVector OverlapDirection = Monster->GetCharacterMovement()->Velocity.GetSafeNormal2D();
	if (!CheckFallable(OverlapDirection, Monster->GetActorLocation()))
	{
		return;
	}
	
	// 여기에 연출 입력
	GetGameInstance()->GetSubsystem<ULLL_FallOutSubsystem>()->FallOutBegin(Monster, OverlapDirection, Monster->GetActorLocation());
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
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("벽 감지. 넉백 연출 미표기")));
		return false;
	}
	return true;
}
