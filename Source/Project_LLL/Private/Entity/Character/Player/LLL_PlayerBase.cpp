// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "Entity/Object/Thrown/PlayerChaseHand/LLL_PlayerChaseHand.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"

ALLL_PlayerBase::ALLL_PlayerBase()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	GoldComponent = CreateDefaultSubobject<ULLL_PlayerGoldComponent>(TEXT("PlayerGoldComponent"));
	CharacterUIManager = CreateDefaultSubobject<ULLL_PlayerUIManager>(TEXT("PlayerUIManageComponent"));
	CharacterAttributeSet = CreateDefaultSubobject<ULLL_PlayerCharacterAttributeSet>(TEXT("PlayerAttributeSet"));

	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerBaseDataAsset>(PATH_PLAYER_DATA, EAssertionLevel::Check);
	CameraDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_CameraDataAsset>(PATH_CAMERA_DATA, EAssertionLevel::Check);

	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);

	GetCharacterMovement()->MaxFlySpeed = 10000.f;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->SetupAttachment(RootComponent);
}

void ALLL_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(CharacterAnimInstance))
	{
		PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(CharacterAnimInstance);
		PlayerAnimInstance->DeadMotionEndedDelegate.AddUObject(this, &ALLL_PlayerBase::DeadMotionEndedHandle);
	}

	if (IsValid(CameraDataAsset))
	{
		Camera->SetFieldOfView(CameraDataAsset->CameraFOV);
		
		SpringArm->TargetArmLength = CameraDataAsset->SpringArmLength;
		SpringArm->SetRelativeRotation(CameraDataAsset->SpringArmAngle);
	}

	ChaseHandActor = Cast<ALLL_PlayerChaseHand>(GetWorld()->SpawnActor(ALLL_PlayerChaseHand::StaticClass()));
	ChaseHandActor->SetOwner(this);

	PlayerUIManager = CastChecked<ULLL_PlayerUIManager>(CharacterUIManager);
	
	if(IsValid(ASC))
	{
		for (const auto SkillAbility : PlayerDataAsset->DefaultSkillAbility)
		{
			if(IsValid(SkillAbility.Value))
			{
				FGameplayAbilitySpec SkillSpec(SkillAbility.Value);
				SkillSpec.InputID = static_cast<int32>(SkillAbility.Key);
				ASC->GiveAbility(SkillSpec);
			}
		}
	}
}

void ALLL_PlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerCollisionDebug())
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

void ALLL_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(PlayerDataAsset->PlayerInputMappingContext, 0);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(PlayerDataAsset->MoveInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::SetMoveInputPressed, true);
	EnhancedInputComponent->BindAction(PlayerDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &ALLL_PlayerBase::MoveAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->MoveInputAction, ETriggerEvent::Completed, this, &ALLL_PlayerBase::SetMoveInputPressed, false);
	EnhancedInputComponent->BindAction(PlayerDataAsset->AttackInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::AttackAction, EAbilityInputName::Attack);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SkillInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::SkillAction, EAbilityInputName::Skill);
	EnhancedInputComponent->BindAction(PlayerDataAsset->ControlChaseInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::ChaseAction, EAbilityInputName::Chase);
	EnhancedInputComponent->BindAction(PlayerDataAsset->DashInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::DashAction, EAbilityInputName::Dash);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractionInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractiveTargetChangeInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractiveTargetChangeAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InventoryInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InventoryAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->PauseInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::PauseAction);
}

void ALLL_PlayerBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	APlayerController* PlayerController = Cast<APlayerController>(NewController);
	if (IsValid(PlayerController))
	{
		PlayerController->SetAudioListenerOverride(SpringArm, FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void ALLL_PlayerBase::AddInteractiveObject(ALLL_InteractiveObject* Object)
{
	InteractiveObjects.Emplace(Object);
	PlayerUIManager->EnableInteractionWidget();
	if (InteractiveObjects.Num() == 1)
	{
		SelectedInteractiveObjectNum = 0;
	}
	PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum], InteractiveObjects.Num() - 1);
}

void ALLL_PlayerBase::RemoveInteractiveObject(ALLL_InteractiveObject* RemoveObject)
{
	if (!InteractiveObjects.IsEmpty())
	{
		for (const auto InteractiveObject : InteractiveObjects)
		{
			if (RemoveObject == InteractiveObject)
			{
				InteractiveObjects.Remove(RemoveObject);
				if(InteractiveObjects.IsEmpty())
				{
					PlayerUIManager->DisableInteractionWidget();
					SelectedInteractiveObjectNum = 0;
				}
				else if (SelectedInteractiveObjectNum >= InteractiveObjects.Num())
				{
					while (SelectedInteractiveObjectNum >= InteractiveObjects.Num())
					{
						SelectedInteractiveObjectNum--;
					}
				}
				break;
			}
		}
		
		if(!InteractiveObjects.IsEmpty())
		{
			PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum], InteractiveObjects.Num() - 1);
		}
	}
}

FVector ALLL_PlayerBase::GetMouseLocation() const
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
		ECC_WorldStatic
	);
	
	if (!bResult)
	{
		return FVector::Zero();
	}
	
	FVector TrueMouseWorldLocation = HitResult.ImpactPoint;
	
	HitResult.Init();
	bResult = false;
	bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		TrueMouseWorldLocation,
		TrueMouseWorldLocation,
		FQuat::Identity,
		ECC_ENEMY_HIT,
		FCollisionShape::MakeSphere(PlayerDataAsset->MouseCursorCorrectRadius),
		Params
		);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug() || ProtoGameInstance->CheckPlayerChaseActionDebug())
		{
			DrawDebugLine(GetWorld(), MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000.f, FColor::Red, false, 3.f);
			DrawDebugPoint(GetWorld(), TrueMouseWorldLocation, 10.f, FColor::Red, false, 3.f);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("마우스 월드 좌표: %f, %f, %f"), TrueMouseWorldLocation.X, TrueMouseWorldLocation.Y, TrueMouseWorldLocation.Z));
			if(bResult)
			{
				DrawDebugSphere(GetWorld(), TrueMouseWorldLocation, PlayerDataAsset->MouseCursorCorrectRadius, 16, FColor::Green, false, 2.f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), TrueMouseWorldLocation, PlayerDataAsset->MouseCursorCorrectRadius, 16, FColor::Red, false, 2.f);
			}
		}
	}
#endif
	
	if(bResult)
	{
		FVector CorrectedMouseLocation = HitResult.GetActor()->GetActorLocation();
		return CorrectedMouseLocation;
	}
	
	return TrueMouseWorldLocation;
}

void ALLL_PlayerBase::MoveAction(const FInputActionValue& Value)
{
	FVector2d MoveInputValue = Value.Get<FVector2D>();
	const float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}

	FRotator CameraRotation = Camera->GetComponentRotation();
	CameraRotation.Pitch = CameraRotation.Roll = 0.f;
	MoveDirection = CameraRotation.RotateVector(FVector(MoveInputValue.X, MoveInputValue.Y, 0.f));
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	if (GetCharacterMovement()->IsWalking())
	{
		AddMovementInput(MoveDirection, 1.f);
	}
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerMovementDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("이동 입력 방향: %f, %f"), MoveDirection.X, MoveDirection.Y));
		}
	}
#endif
}

void ALLL_PlayerBase::DashAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	const int32 InputID = static_cast<int32>(InputName);
	if(FGameplayAbilitySpec* DashSpec = ASC->FindAbilitySpecFromInputID(InputID))
	{
		DashSpec->InputPressed = true;
		if (DashSpec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*DashSpec);
		}
		else
		{
			ASC->TryActivateAbility(DashSpec->Handle);
		}
	}
}

void ALLL_PlayerBase::AttackAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	const int32 InputID = static_cast<int32>(InputName);
	if(FGameplayAbilitySpec* AttackSpec = ASC->FindAbilitySpecFromInputID(InputID))
	{
		AttackSpec->InputPressed = true;
		if (AttackSpec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*AttackSpec);
		}
		else
		{
			ASC->TryActivateAbility(AttackSpec->Handle);
		}
	}
}

void ALLL_PlayerBase::ChaseAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	const int32 InputID = static_cast<int32>(InputName);
	if(const FGameplayAbilitySpec* ChaseSpec = ASC->FindAbilitySpecFromInputID(InputID))
	{
		ASC->TryActivateAbility(ChaseSpec->Handle);
	}
}

void ALLL_PlayerBase::SkillAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	const int32 InputID = static_cast<int32>(InputName);
	if(FGameplayAbilitySpec* SkillSpec = ASC->FindAbilitySpecFromInputID(InputID))
	{
		SkillSpec->InputPressed = true;
		if (SkillSpec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*SkillSpec);
		}
		else
		{
			ASC->TryActivateAbility(SkillSpec->Handle);
		}
	}
}

void ALLL_PlayerBase::InteractAction(const FInputActionValue& Value)
{
	if (!InteractiveObjects.Num())
	{
		return;
	}
	InteractiveObjects[SelectedInteractiveObjectNum]->InteractiveEvent();
}

void ALLL_PlayerBase::InteractiveTargetChangeAction(const FInputActionValue& Value)
{
	if (!InteractiveObjects.Num())
	{
		return;
	}
	
	SelectedInteractiveObjectNum++;
	if (SelectedInteractiveObjectNum >= InteractiveObjects.Num())
	{
		SelectedInteractiveObjectNum = 0;
	}
	PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum], InteractiveObjects.Num() - 1);
	
}

void ALLL_PlayerBase::InventoryAction(const FInputActionValue& Value)
{
	PlayerUIManager->ToggleInventoryWidget();
}

void ALLL_PlayerBase::PauseAction(const FInputActionValue& Value)
{
	PlayerUIManager->TogglePauseWidget(bIsDead);
}

void ALLL_PlayerBase::PlayerRotateToMouseCursor()
{
	const FVector MouseWorldLocation = GetMouseLocation();
	FVector ViewDirection = (MouseWorldLocation - GetActorLocation()).GetSafeNormal();
	ViewDirection.Z = 0.f;
	SetActorRotation(ViewDirection.Rotation());
	
}

void ALLL_PlayerBase::Dead()
{
	Super::Dead();

	// TODO: 목숨 같은거 생기면 사이에 추가하기
	
	DisableInput(Cast<APlayerController>(GetController()));

	PlayerAnimInstance = CastChecked<ULLL_PlayerAnimInstance>(CharacterAnimInstance);
	PlayerAnimInstance->PlayDeadAnimation();
}

void ALLL_PlayerBase::DeadMotionEndedHandle()
{
	PlayerUIManager->TogglePauseWidget(bIsDead);
}
