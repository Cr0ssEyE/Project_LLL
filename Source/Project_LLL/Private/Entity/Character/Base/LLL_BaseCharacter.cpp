// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "FMODAudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "DataTable/LLL_FModParameterDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ASC/LLL_BaseASC.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Util/LLL_ExecuteCueHelper.h"

// Sets default values
ALLL_BaseCharacter::ALLL_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsDead = false;

	ASC = CreateDefaultSubobject<ULLL_BaseASC>(TEXT("AbilitySystem"));
	FModAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FModAudioComponent"));
	
	FModAudioComponent->SetupAttachment(RootComponent);

	Level = 1;

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	bIsSpawned = false;
#endif
}

void ALLL_BaseCharacter::PostLoad()
{
	Super::PostLoad();
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	SetDefaultInformation();
#endif
}

void ALLL_BaseCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	SetDefaultInformation();
#endif
}

void ALLL_BaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SetDefaultInformation();
}

void ALLL_BaseCharacter::SetDefaultInformation()
{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(bIsSpawned)
	{
		return;
	}
#endif

	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(1);
	
	if (IsValid(CharacterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(CharacterDataAsset->CollisionSize.Y, CharacterDataAsset->CollisionSize.X);

		GetMesh()->SetSkeletalMesh(CharacterDataAsset->SkeletalMesh);
		GetMesh()->SetRelativeScale3D(CharacterDataAsset->MeshSize);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -CharacterDataAsset->CollisionSize.X));

		UClass* AnimBlueprint = CharacterDataAsset->AnimInstance.LoadSynchronous();
		if (IsValid(AnimBlueprint))
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprint);
			CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		}

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->FallingLateralFriction = 3.0f;

		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		bIsSpawned = true;
#endif
	}
}

// Called when the game starts or when spawned
void ALLL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if(IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		for (const auto ActiveAbility : CharacterDataAsset->ActiveGameplayAbility)
		{
			if(IsValid(ActiveAbility))
			{
				FGameplayAbilitySpec AbilitySpec(ActiveAbility);
				ASC->GiveAbility(AbilitySpec);
			}
		}
		
		for (const auto PassiveAbility : CharacterDataAsset->PassiveGameplayAbility)
		{
			if(IsValid(PassiveAbility))
			{
				FGameplayAbilitySpec AbilitySpec(PassiveAbility);
				ASC->GiveAbility(AbilitySpec);
			}
		}

		UpdateWidgetDelegate.Broadcast();
	}

	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]{
		InitAttributeSet();
		const ULLL_CharacterAttributeSetBase* CharacterAttributeSetBase = CastChecked<ULLL_CharacterAttributeSetBase>(ASC->GetAttributeSet(ULLL_CharacterAttributeSetBase::StaticClass()));
		GetCharacterMovement()->MaxAcceleration = CharacterAttributeSetBase->GetAccelerateSpeed();
		GetCharacterMovement()->GroundFriction = CharacterAttributeSetBase->GetGroundFriction();
		GetCharacterMovement()->RotationRate = FRotator(0.f, CharacterAttributeSetBase->GetTurnSpeed() * 360.f, 0.f);
	}));
}

void ALLL_BaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentPitch != Pitch)
	{
		CurrentPitch = FMath::FInterpTo(CurrentPitch, Pitch, DeltaSeconds, 1.0f);
		
		FModAudioComponent->SetPitch(CurrentPitch);
	}
}

void ALLL_BaseCharacter::InitAttributeSet()
{
	UE_LOG(LogTemp, Log, TEXT("%s 어트리뷰트 초기화 리스트"), *GetName())

	TArray<UAttributeSet*> SpawnedAttributes = ASC->GetSpawnedAttributes();
	for (const auto SpawnedAttribute : SpawnedAttributes)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s"), *SpawnedAttribute->GetName())
	}
}

void ALLL_BaseCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (HitLocation.Z >= GetActorLocation().Z)
	{
		OtherActorCollidedDelegate.Broadcast(this, Other);
	}

	const ECollisionResponse Response = Other->GetComponentsCollisionResponseToChannel(ECC_WALL_ONLY);
	// Static Actor 중에서 벽과 바닥을 구분하는 좋은 방법이 뭐가 있을지 고민. 지금은 머릿속에서 생각나는게 이게 최선
	if (Response == ECR_Block && FMath::Abs(HitNormal.Z) < 0.2f)
	{
		ASC->AddLooseGameplayTag(TAG_GAS_COLLIDE_WALL);

		// 게임플레이 이펙트 없이 인스턴스 태그를 부착하기 위해 타이머 델리게이트로 다음 틱에서 제거
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [&]()
		{
			if(IsValid(ASC))
			{
				ASC->RemoveLooseGameplayTag(TAG_GAS_COLLIDE_WALL);
			}
		}));
	}
}

void ALLL_BaseCharacter::Damaged()
{
	FLLL_ExecuteCueHelper::ExecuteCue(this, CharacterDataAsset->DamagedCueTag);
}

void ALLL_BaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}

	CharacterAnimInstance->StopAllMontages(1.0f);

	GetCapsuleComponent()->SetCollisionProfileName(CP_RAGDOLL);
	GetMesh()->SetCollisionProfileName(CP_RAGDOLL);
	
	bIsDead = true;

	CharacterDeadDelegate.Broadcast(this);

	FLLL_ExecuteCueHelper::ExecuteCue(this, CharacterDataAsset->DeadCueTag);
}
