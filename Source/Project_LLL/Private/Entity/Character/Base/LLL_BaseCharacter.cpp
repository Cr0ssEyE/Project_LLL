// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FMODAudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Game/LLL_GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/ASC/LLL_BaseASC.h"
#include "GAS/Attribute/Character/Base/LLL_CharacterAttributeSetBase.h"
#include "Util/LLL_FModPlayHelper.h"

ALLL_BaseCharacter::ALLL_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bIsDead = false;

	ASC = CreateDefaultSubobject<ULLL_BaseASC>(TEXT("AbilitySystem"));
	
	FModAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FModAudioComponent"));
	FModAudioComponent->SetupAttachment(RootComponent);

	Level = 1;
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
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(2);
	
	if (IsValid(CharacterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(CharacterDataAsset->CollisionSize.Y, CharacterDataAsset->CollisionSize.X);

		GetMesh()->SetSkeletalMesh(CharacterDataAsset->SkeletalMesh);
		GetMesh()->SetRelativeScale3D(CharacterDataAsset->MeshSize);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -CharacterDataAsset->CollisionSize.X));
		GetMesh()->SetBoundsScale(100.f);
		
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
	}
}

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

void ALLL_BaseCharacter::InitAttributeSet()
{
	UE_LOG(LogTemp, Log, TEXT("%s 어트리뷰트 초기화 리스트"), *GetName())

	TArray<UAttributeSet*> SpawnedAttributes = ASC->GetSpawnedAttributes();
	for (const auto SpawnedAttribute : SpawnedAttributes)
	{
		UE_LOG(LogTemp, Log, TEXT("- %s"), *SpawnedAttribute->GetName())
	}
}

void ALLL_BaseCharacter::AddNiagaraComponent(UNiagaraComponent* InNiagaraComponent)
{
	NiagaraComponents.Remove(nullptr);
	NiagaraComponents.Emplace(InNiagaraComponent);
}

void ALLL_BaseCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (HitLocation.Z < GetActorLocation().Z)
	{
		return;
	}

	LastCollideLocation = HitLocation;
	LastCollideLocation.Z = GetActorLocation().Z;
	
	OtherActorCollidedDelegate.Broadcast(this, Other);

	const ECollisionResponse WallResponse = Other->GetComponentsCollisionResponseToChannel(ECC_WALL_ONLY);
	const ECollisionResponse FieldResponse = Other->GetComponentsCollisionResponseToChannel(ECC_TRACE_FIELD);
	
	if (WallResponse == ECR_Block && FieldResponse == ECR_Ignore)
	{
		FGameplayEventData PayloadData;
		PayloadData.Instigator = Other;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, TAG_GAS_COLLIDE_WALL, PayloadData);
	}
}

void ALLL_BaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}
	
	bIsDead = true;

	CharacterDeadDelegate.Broadcast(this);

	ASC->CancelAbilities();
}

void ALLL_BaseCharacter::SetParameter(EFModParameter FModParameter, float value) const
{
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	for (const auto FModParameterData : GameInstance->GetFModParameterDataArray())
	{
		if (FModParameterData.Parameter != FModParameter)
		{
			continue;
		}

		FModAudioComponent->SetParameter(FModParameterData.Name, value);
	}
}
