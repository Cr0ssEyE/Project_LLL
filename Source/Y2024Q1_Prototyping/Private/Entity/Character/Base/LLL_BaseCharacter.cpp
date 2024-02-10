// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALLL_BaseCharacter::ALLL_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsDead = false;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

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

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = CharacterDataAsset->MoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = CharacterDataAsset->AccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = CharacterDataAsset->GroundFriction;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, CharacterDataAsset->TurnSpeed * 360.f, 0.f);
		GetCharacterMovement()->FallingLateralFriction = 3.0f;

		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;

		MaxHealthAmount = CharacterDataAsset->Health;
		CurrentHealthAmount = MaxHealthAmount;
		MaxShieldAmount = CharacterDataAsset->ShieldAmount;
		CurrentShieldAmount = MaxShieldAmount;
		OffensePower = CharacterDataAsset->OffensePower;
		AttackDistance = CharacterDataAsset->AttackDistance;
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		bIsSpawned = true;
#endif
	}
}

// Called when the game starts or when spawned
void ALLL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(!IsValid(CharacterAnimInstance))
	{
		CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());;
	}

	if (IsValid(CharacterAnimInstance))
	{
		CharacterAnimInstance->DeadMotionEndedDelegate.AddUObject(this, &ALLL_BaseCharacter::DeadMontageEndEvent);
	}

	if(IsValid(ASC))
	{
		ASC->InitAbilityActorInfo(this, this);

		for (const auto BaseAbility : CharacterDataAsset->DefaultGameplayAbility)
		{
			if(IsValid(BaseAbility))
			{
				FGameplayAbilitySpec AbilitySpec(BaseAbility);
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

// Called every frame
void ALLL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ALLL_BaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(CurrentShieldAmount > 0)
	{
		CurrentShieldAmount -= DamageAmount;
		if(CurrentShieldAmount <= 0)
		{
			CurrentShieldAmount = 0;
		}
	}
	else
	{
		CurrentHealthAmount -= DamageAmount;
		if(CurrentHealthAmount <= 0)
		{
			CurrentHealthAmount = 0;
			Dead();
		}
		else
		{
			Damaged();
		}
	}

	return ActualDamage;
}

void ALLL_BaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}

	CharacterAnimInstance->PlayDeadAnimation();
	
	CurrentHealthAmount = 0;
	bIsDead = true;

	CharacterDeadDelegate.Broadcast(this);
}

void ALLL_BaseCharacter::DeadMontageEndEvent()
{
	// TODO: 화면 페이드, 결과창 출력 등등. 임시로 Destroy 처리
	Destroy();
}
