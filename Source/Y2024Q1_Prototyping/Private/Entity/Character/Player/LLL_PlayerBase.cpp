// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLLConstructorHelper.h"

ALLL_PlayerBase::ALLL_PlayerBase()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	
	PlayerBaseDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerBaseDataAsset>(TEXT("/Script/Y2024Q1_Prototyping.LLL_PlayerBaseDataAsset'/Game/7-Player-View-Movement/Assets/PlayerBaseDataAsset.PlayerBaseDataAsset'"), EAssertionLevel::Check);
	
	if(IsValid(PlayerBaseDataAsset))
	{
		DashSpeed = PlayerBaseDataAsset->PlayerBaseDashSpeed;
		
		GetCapsuleComponent()->SetCapsuleSize(PlayerBaseDataAsset->PlayerCollisionSize.Y, PlayerBaseDataAsset->PlayerCollisionSize.X);
		
		GetMesh()->SetSkeletalMesh(PlayerBaseDataAsset->CharacterBaseMesh);
		GetMesh()->SetAnimInstanceClass(PlayerBaseDataAsset->CharacterAnimBlueprint);
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		GetMesh()->AddRelativeLocation(FVector(0.f, 0.f, -PlayerBaseDataAsset->PlayerCollisionSize.X));

		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed = PlayerBaseDataAsset->PlayerBaseMoveSpeed;
		GetCharacterMovement()->MaxAcceleration = AccelerateSpeed = PlayerBaseDataAsset->PlayerBaseAccelerateSpeed;
		GetCharacterMovement()->GroundFriction = GroundFriction = PlayerBaseDataAsset->PlayerBaseGroundFriction;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, PlayerBaseDataAsset->PlayerBaseTurnSpeed * 360.f);
		bUseControllerRotationYaw = false;
		bUseControllerRotationPitch = false;
		bUseControllerRotationRoll = false;
		
		Camera->SetFieldOfView(PlayerBaseDataAsset->CameraFOV);
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
		Camera->bUsePawnControlRotation = false;
		
		SpringArm->TargetArmLength = PlayerBaseDataAsset->SpringArmLength;
		SpringArm->SetRelativeRotation(FRotator(PlayerBaseDataAsset->SpringArmAngle, 0.f, 0.f));
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;
		SpringArm->SetupAttachment(RootComponent);
	}
}

void ALLL_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALLL_PlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerCollisionDebug())
		{
			GetCapsuleComponent()->SetHiddenInGame(false);
		}
		else
		{
			GetCapsuleComponent()->SetHiddenInGame(true);
		}
	}
#endif
}

float ALLL_PlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 무적 또는 데미지 처리 무시는 해당 구문에서 추가
	if(bIsInvincibleOnDashing)
	{
		return 0;
	}
	
	return 0;
}

void ALLL_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(PlayerBaseDataAsset->PlayerInputMappingContext, 0);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &ALLL_PlayerBase::MoveAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->AttackInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::AttackAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->DashInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::DashAction);
}

void ALLL_PlayerBase::MoveAction(const FInputActionValue& Value)
{
	FVector2d MoveInputValue = Value.Get<FVector2D>();
	float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}
	
	FVector MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, 1.f);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerMovementDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("플레이어 캐릭터 방향 X: %f, Y: %f"), MoveDirection.X, MoveDirection.Y));
		}
	}
#endif
}

void ALLL_PlayerBase::DashAction(const FInputActionValue& Value)
{
	LaunchCharacter(GetActorForwardVector() * (DashSpeed * 1000.f), true, true);
}

void ALLL_PlayerBase::AttackAction(const FInputActionValue& Value)
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			MouseWorldLocation,
			MouseWorldLocation + MouseWorldDirection * 10000.f,
			ECC_Visibility
		);
	
	if(bResult)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerAttackDebug())
			{
				DrawDebugLine(GetWorld(), MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000.f, FColor::Red, false, 3.f);
				DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, 3.f);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("마우스 월드 좌표: %f, %f, %f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z));
			}
		}
#endif
		
		FVector AttackDirection = (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal();
		AttackDirection.Z = 0.f;
		SetActorRotation(AttackDirection.Rotation());
	}
}

void ALLL_PlayerBase::SkillAction(const FInputActionValue& Value)
{
	
}

void ALLL_PlayerBase::InteractAction(const FInputActionValue& Value)
{
	
}

void ALLL_PlayerBase::InventoryAction(const FInputActionValue& Value)
{
	
}

void ALLL_PlayerBase::PauseAction(const FInputActionValue& Value)
{
	
}

void ALLL_PlayerBase::CheckDashInvincibilityTime()
{
	
}

