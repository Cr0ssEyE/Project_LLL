// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Base/LLL_BaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALLL_BaseCharacter::ALLL_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bIsDead = false;

	if (IsValid(CharacterDataAsset))
	{
		GetCapsuleComponent()->SetCapsuleSize(CharacterDataAsset->CollisionSize.Y, CharacterDataAsset->CollisionSize.X);

		GetMesh()->SetSkeletalMesh(CharacterDataAsset->SkeletalMesh);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -CharacterDataAsset->CollisionSize.X));

		UClass* AnimBlueprint = CharacterDataAsset->AnimInstance.LoadSynchronous();
		if (IsValid(AnimBlueprint))
		{
			GetMesh()->SetAnimInstanceClass(AnimBlueprint);
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
	}
}

// Called when the game starts or when spawned
void ALLL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALLL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALLL_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALLL_BaseCharacter::Dead()
{
	if (bIsDead)
	{
		return;
	}

	// Todo: 사망 애니메이션 재생
	
	CurrentHealthAmount = 0;
	bIsDead = true;
}
