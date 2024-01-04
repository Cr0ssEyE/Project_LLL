// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
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

	PlayerUIManager = CreateDefaultSubobject<ULLL_PlayerUIManager>(TEXT("PlayerUIManageComponent"));
	
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
		GetCharacterMovement()->RotationRate = FRotator(0.f, PlayerBaseDataAsset->PlayerBaseTurnSpeed * 360.f, 0.f);
		GetCharacterMovement()->FallingLateralFriction = 3.0f;
		
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

		MaxDashCount = PlayerBaseDataAsset->DashBaseCount;
		DashInputCheckTime = PlayerBaseDataAsset->DashInputCheckTime;
		DashCoolDownSeconds = PlayerBaseDataAsset->DashBaseCoolDownSeconds;
		DashInvincibleTime = PlayerBaseDataAsset->DashBaseInvincibleTime;
		
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
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->SkillInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::SkillAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->InteractionInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->InteractiveTargetChangeInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractiveTargetChangeAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->InventoryInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InventoryAction);
	EnhancedInputComponent->BindAction(PlayerBaseDataAsset->PauseInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::PauseAction);
}

void ALLL_PlayerBase::AddInteractableObject(ALLL_InteractiveObject* Object)
{
	InteractiveObjects.Emplace(Object);
	PlayerUIManager->EnableInteractionWidget();
}

void ALLL_PlayerBase::RemoveInteractableObject(ALLL_InteractiveObject* RemoveObject)
{
	if(!InteractiveObjects.IsEmpty())
	{
		for(const auto InteractiveObject : InteractiveObjects)
		{
			if(RemoveObject == InteractiveObject)
			{
				InteractiveObjects.Remove(RemoveObject);
				if(InteractiveObjects.IsEmpty())
				{
					PlayerUIManager->DisableInteractionWidget();
					SelectedInteractiveObjectNum = 0;
				}
				else if(SelectedInteractiveObjectNum >= InteractiveObjects.Num())
				{
					while (SelectedInteractiveObjectNum < InteractiveObjects.Num())
					{
						SelectedInteractiveObjectNum--;
					}
					PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum]);
				}
			}
		}
	}
}

void ALLL_PlayerBase::MoveAction(const FInputActionValue& Value)
{
	FVector2d MoveInputValue = Value.Get<FVector2D>();
	float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}
	
	MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.f);
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
	if(DashDisabledTime > 0 || CurrentDashCount >= MaxDashCount)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				if(CurrentDashCount >= MaxDashCount)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 횟수 사용 후 입력. 현재 사용 횟수|최대 사용 횟수 : %d, %d"), CurrentDashCount, MaxDashCount));
				}
				else if(DashDisabledTime > 0)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 대기시간 도중 입력. 현재 남은 대기시간 : %f"), DashCoolDownSeconds - DashDisabledTime));
				}
			}
		}
#endif
		return;
	}
	
	CurrentDashCount++;
	DashElapsedTime = 0;
	DashDisabledTime = 0;
	
	LaunchCharacter(MoveDirection * (DashSpeed * 1000.f), true, true);
	
	bIsInvincibleOnDashing = true;
	if(GetWorldTimerManager().IsTimerActive(DashStateCheckTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DashStateCheckTimerHandle);
	}
	GetWorldTimerManager().SetTimer(DashStateCheckTimerHandle, this, &ALLL_PlayerBase::CheckDashElapsedTime, 0.01f, true);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(ProtoGameInstance->CheckPlayerDashDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 입력. 현재 사용 횟수|최대 사용 횟수 : %d, %d"), CurrentDashCount, MaxDashCount));
		}
	}
#endif
}

void ALLL_PlayerBase::AttackAction(const FInputActionValue& Value)
{
	CharacterRotateToCursor();
	// TODO: 공격 처리용 가상 함수 만들어서 붙이기
}

void ALLL_PlayerBase::SkillAction(const FInputActionValue& Value)
{
	CharacterRotateToCursor();
	// TODO: 스킬 처리용 가상 함수 만들어서 붙이기
}

void ALLL_PlayerBase::InteractAction(const FInputActionValue& Value)
{
	if(!InteractiveObjects.Num())
	{
		return;
	}
	InteractiveObjects[SelectedInteractiveObjectNum]->InteractiveEvent();
}

void ALLL_PlayerBase::InteractiveTargetChangeAction(const FInputActionValue& Value)
{
	if(!InteractiveObjects.Num())
	{
		return;
	}
	
	if(SelectedInteractiveObjectNum < InteractiveObjects.Num())
	{
		SelectedInteractiveObjectNum++;
		if(SelectedInteractiveObjectNum == InteractiveObjects.Num())
		{
			SelectedInteractiveObjectNum = 0;
		}
	}
}

void ALLL_PlayerBase::InventoryAction(const FInputActionValue& Value)
{
	PlayerUIManager->ToggleInventoryWidget();
}

void ALLL_PlayerBase::PauseAction(const FInputActionValue& Value)
{
	PlayerUIManager->TogglePauseWidget();
}

void ALLL_PlayerBase::CharacterRotateToCursor()
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
			if(ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
			{
				DrawDebugLine(GetWorld(), MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000.f, FColor::Red, false, 3.f);
				DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, 3.f);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("마우스 월드 좌표: %f, %f, %f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z));
			}
		}
#endif
		
		FVector ViewDirection = (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal();
		ViewDirection.Z = 0.f;
		SetActorRotation(ViewDirection.Rotation());
	}
}

void ALLL_PlayerBase::CheckDashElapsedTime()
{
	DashElapsedTime += GetWorld()->DeltaTimeSeconds;
	if(DashElapsedTime >= DashInvincibleTime && bIsInvincibleOnDashing)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 무적시간 종료")));
			}
		}
#endif

		bIsInvincibleOnDashing = false;
	}

	if(DashElapsedTime >= DashInputCheckTime)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 연속사용 입력대기 종료")));
			}
		}
#endif
		
		CurrentDashCount = 0;
		GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::CheckDashDelay);
		GetWorldTimerManager().ClearTimer(DashStateCheckTimerHandle);
	}
}

void ALLL_PlayerBase::CheckDashDelay()
{
	DashDisabledTime += GetWorld()->DeltaTimeSeconds;
	if(DashDisabledTime >= DashCoolDownSeconds || bIsInvincibleOnDashing)
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("돌진 재사용 대기시간 종료")));
			}
		}
#endif
		
		DashDisabledTime = 0.f;
		return;
	}
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::CheckDashDelay);
}

