// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbilitiesModule.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "DataAsset/LLL_PlayerChaseHandDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Object/Thrown/LLL_PlayerChaseHandAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerChaseHand::ALLL_PlayerChaseHand()
{
	ThrownObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerChaseHandDataAsset>(PATH_PLAYER_CHASE_HAND_DATA, EAssertionLevel::Check);
	BaseObjectDataAsset = Cast<ULLL_BaseObjectDataAsset>(ThrownObjectDataAsset);
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_PlayerChaseHandAttributeSet>(TEXT("ChaseHandAttributeSet"));

	HandMesh->SetSkeletalMesh(ThrownObjectDataAsset->SkeletalMesh);
	HandMesh->SetRelativeScale3D(ThrownObjectDataAsset->MeshScale);
	HandMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	
	// 이후 BeginPlay에서 InitEffect를 통해 실제 사용하는 값으로 초기화 해준다. 해당 매직넘버는 비정상적인 동작 방지용
	HandCollision->SetSphereRadius(50.f);
	RootComponent = HandCollision;
	HandMesh->SetupAttachment(RootComponent);

	HandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandCollision->SetCollisionProfileName(CP_NO_COLLISION);
	HandMesh->SetHiddenInGame(true);
	
	bIsGrabbed = false;
}

void ALLL_PlayerChaseHand::SetHiddenState()
{
	bIsGrabbed = false;
	HandCollision->SetCollisionObjectType(ECC_PLAYER);
	HandCollision->SetCollisionProfileName(CP_NO_COLLISION);
	HandMesh->SetHiddenInGame(true);

	ProjectileMovementComponent->Velocity = FVector::Zero();
	ProjectileMovementComponent->Deactivate();
	
	SetActorLocation(GetOwner()->GetActorLocation());
}

void ALLL_PlayerChaseHand::BeginPlay()
{
	Super::BeginPlay();

	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(ASC, FName("Player"), 1.f, true);
}

void ALLL_PlayerChaseHand::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	// 그랩&추격 액션 후 회수 처리
	if ( Cast<ALLL_PlayerBase>(OtherActor) && HandCollision->GetCollisionObjectType() == ECC_PLAYER_CHECK)
	{
		SetHiddenState();
		ReleaseCompleteDelegate.Broadcast();
	}
}

void ALLL_PlayerChaseHand::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	// ThrownObject 쪽 NotifyHit 함수 별도로 옮기기 전까진 부모 클래스 함수 호출 막아놓기
	// Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (bIsGrabbed)
	{
		return;
	}
	
	const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Other);
	if (IsValid(Monster))
	{
		// PGA_WireHandGrab
		if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_CHASE_GRAB)))
		{
			GrabbedActor = Other;
			bIsGrabbed = true;
			OnGrabbedDelegate.Broadcast();
			return;
		}
	}

	// 그랩 불가능 대상 충돌시 회수 처리
	if (Cast<ALLL_PlayerBase>(Other) || Other->GetComponentsCollisionResponseToChannel(ECC_TRACE_FIELD) == ECR_Block)
	{
		return;
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("벽과 충돌 체크")));
		}
	}
#endif
	
	SetHiddenState();
	ReleaseCompleteDelegate.Broadcast();
}
