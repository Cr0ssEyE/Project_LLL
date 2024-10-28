// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_FallableWallGimmick.h"

#include "FMODAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Ability/Character/Monster/LLL_MGA_SetFallableState.h"
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

	FallTimeDilation = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->FallEventTimeDilation;
	FallEventDuration = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->FallEventDuration;
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
	
	Monster->GetAbilitySystemComponent()->RemoveLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE, 99);
	Monster->CustomTimeDilation = 1.f;
	Monster->GetCharacterMovement()->Velocity = FVector::Zero();
	Monster->GetAbilitySystemComponent()->CancelAbility(Cast<UGameplayAbility>(ULLL_MGA_SetFallableState::StaticClass()->GetDefaultObject()));
	
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
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("벽 감지. 넉백 연출 미표기")));
		return false;
	}
	return true;
}

void ALLL_FallableWallGimmick::FallOutBegin(AActor* Actor, FVector HitNormal, FVector HitLocation)
{
	// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("넉백 연출 시작")));
	const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Actor);
	if (!IsValid(GetWorld()) || !IsValid(Monster))
	{
		return;
	}
	
	GetWorldSettings()->SetTimeDilation(FallTimeDilation);
	CustomTimeDilation = 1.f / GetWorldSettings()->TimeDilation;
	UNiagaraSystem* WallCrashNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->InvisibleWallCrashNiagaraSystem;
	UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WallCrashNiagaraSystem, HitLocation, HitNormal.Rotation());
	NiagaraComponent->SetCustomTimeDilation(CustomTimeDilation);
	AddNiagaraComponent(NiagaraComponent);

	UNiagaraSystem* TrailNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->FallTrailNiagaraSystem;
	UNiagaraFunctionLibrary::SpawnSystemAttached(TrailNiagaraSystem, Monster->GetMesh(), SOCKET_CHEST, FVector::Zero(), FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	
	FTimerHandle DilationTimerHandle;
	GetWorldTimerManager().SetTimer(DilationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("넉백 연출 종료")));
		GetWorldSettings()->SetTimeDilation(1.f);
		CustomTimeDilation = 1.f;
		FallOutStart(Actor, HitNormal);
	}), FallEventDuration * GetWorldSettings()->TimeDilation, false);
}

void ALLL_FallableWallGimmick::FallOutStart(AActor* Actor, FVector HitNormal)
{
	ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Actor);
	if (!IsValid(GetWorld()) || !IsValid(Monster))
	{
		return;
	}
	
	CustomTimeDilation = 1.f;
	Monster->CustomTimeDilation = 1.f;
	
	Monster->GetCapsuleComponent()->SetCollisionProfileName(CP_OVERLAP_ALL);
	GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&, HitNormal, Monster]
	{
		if (!IsValid(GetWorld()) || !IsValid(Monster))
		{
			return;
		}
		
		const float DefaultFallKnockBackPower = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->DefaultFallKnockBackPower;
		float StackedKnockBackPower = FMath::Max(Monster->GetKnockBackedPower() * 5.f, DefaultFallKnockBackPower);
		FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(HitNormal, StackedKnockBackPower);
		Monster->AddKnockBackVelocity(LaunchVelocity, -1.f);
	}));

	/*FTimerHandle DilationTimerHandle;
	float FallVFXDelay = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->FallExplosionDelayTime;
	GetWorldTimerManager().SetTimer(DilationTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]
	{
		if (!IsValid(GetWorld()) || !IsValid(Monster))
		{
			return;
		}

		SpawnFallExplosionVFX(Monster);
	}), FallVFXDelay, false);*/
}

void ALLL_FallableWallGimmick::SpawnFallExplosionVFX(ALLL_MonsterBase* Monster)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(PlayerController))
	{
		return;
	}
	
	ALLL_PlayerBase* PlayerCharacter = Cast<ALLL_PlayerBase>(PlayerController->GetCharacter());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
		
	FVector OtherActorWorldPosition = Monster->GetActorLocation();
	OtherActorWorldPosition.Z = PlayerCharacter->GetActorLocation().Z;
	FVector FallOutVFXDirection = (OtherActorWorldPosition - PlayerCharacter->GetActorLocation()).GetSafeNormal2D();

	int32 ViewportX;
	int32 ViewportY;
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	FVector FallOutVFXLocation = FallOutVFXDirection * FVector(ViewportY, ViewportX, 1.f) + PlayerCharacter->GetActorLocation();
	FRotator FallOutVFXRotation = (FallOutVFXLocation - PlayerCharacter->GetActorLocation()).GetSafeNormal().Rotation();
		
	UNiagaraSystem* ExplosionNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->FallExplosionNiagaraSystem;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionNiagaraSystem, FallOutVFXLocation, FallOutVFXRotation);
}

