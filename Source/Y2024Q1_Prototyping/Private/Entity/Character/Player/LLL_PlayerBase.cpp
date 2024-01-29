// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_MonatgeSectionName.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "DataAsset/LLL_WeaponBaseDataAsset.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Character/Player/LLL_PlayerWeaponComponent.h"
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
	PlayerUIManager = CreateDefaultSubobject<ULLL_PlayerUIManager>(TEXT("PlayerUIManageComponent"));
	PlayerWeaponComponent = CreateDefaultSubobject<ULLL_PlayerWeaponComponent>(TEXT("PlayerWeaponComponent"));

	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerBaseDataAsset>(PATH_PLAYER_DATA, EAssertionLevel::Check);
	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
	if (IsValid(CharacterDataAsset))
	{
		DashSpeed = PlayerDataAsset->DashSpeed;
		
		GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
		
		Camera->SetFieldOfView(PlayerDataAsset->CameraFOV);
		Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
		Camera->bUsePawnControlRotation = false;
		
		SpringArm->TargetArmLength = PlayerDataAsset->SpringArmLength;
		SpringArm->SetRelativeRotation(FRotator(PlayerDataAsset->SpringArmAngle, 0.f, 0.f));
		SpringArm->bDoCollisionTest = false;
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bInheritRoll = false;
		SpringArm->SetupAttachment(RootComponent);
		
		MaxDashCount = PlayerDataAsset->DashBaseCount;
		DashInputCheckTime = PlayerDataAsset->DashInputCheckTime;
		DashCoolDownSeconds = PlayerDataAsset->DashBaseCoolDownSeconds;
		DashInvincibleTime = PlayerDataAsset->DashBaseInvincibleTime;
		
		if(IsValid(PlayerDataAsset->DefaultWeaponBaseDataAsset))
		{
			PlayerWeaponComponent->SetupWeaponInfo(PlayerDataAsset->DefaultWeaponBaseDataAsset);
			MaxComboActionCount = PlayerDataAsset->DefaultWeaponBaseDataAsset->WeaponAttackActionCount;
		}
		else
		{
			// 테스트용
			MaxComboActionCount = 3;
		}
	}
}

void ALLL_PlayerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(PlayerAnimInstance = Cast<ULLL_PlayerAnimInstance>(GetMesh()->GetAnimInstance())))
	{
		PlayerAnimInstance->SetDataAsset(CharacterDataAsset);
		PlayerAnimInstance->AttackComboCheckDelegate.AddUObject(this, &ALLL_PlayerBase::SetAttackComboCheckState);
		PlayerAnimInstance->AttackHitCheckDelegate.AddUObject(this, &ALLL_PlayerBase::SetAttackHitCheckState);
	}
	
	PlayerUIManager->UpdateStatusWidget(MaxHealthAmount, CurrentHealthAmount, MaxShieldAmount, CurrentShieldAmount);
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

float ALLL_PlayerBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 무적 또는 데미지 처리 무시는 해당 구문에서 추가
	if (bIsInvincibleOnDashing)
	{
		return 0;
	}
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
			// TODO: 목숨 같은거 생기면 사이에 추가하기
			Dead();
		}
	}
	PlayerUIManager->UpdateStatusWidget(MaxHealthAmount, CurrentHealthAmount, MaxShieldAmount, CurrentShieldAmount);
	return 0;
}

void ALLL_PlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	const APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(PlayerDataAsset->PlayerInputMappingContext, 0);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComponent->BindAction(PlayerDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &ALLL_PlayerBase::MoveAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->AttackInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::AttackAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->DashInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::DashAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SkillInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::SkillAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractionInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractiveTargetChangeInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractiveTargetChangeAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InventoryInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InventoryAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->PauseInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::PauseAction);
}

void ALLL_PlayerBase::AddInteractableObject(ALLL_InteractiveObject* Object)
{
	InteractiveObjects.Emplace(Object);
	PlayerUIManager->EnableInteractionWidget();
	if(InteractiveObjects.Num() == 1)
	{
		SelectedInteractiveObjectNum = 0;
	}
	PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum], InteractiveObjects.Num() - 1);
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

void ALLL_PlayerBase::MoveAction(const FInputActionValue& Value)
{
	FVector2d MoveInputValue = Value.Get<FVector2D>();
	const float MovementVectorSizeSquared = MoveInputValue.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MoveInputValue.Normalize();
	}
	
	MoveDirection = FVector(MoveInputValue.X, MoveInputValue.Y, 0.f);

	if(bIsAttackActionOnGoing)
	{
		return;
	}
 	if(GetMesh()->GetAnimInstance()->IsAnyMontagePlaying())
	{
		GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
		ClearState();
	}
	
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, 1.f);

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerMovementDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("플레이어 캐릭터 방향 X: %f, Y: %f"), MoveDirection.X, MoveDirection.Y));
		}
	}
#endif
}

void ALLL_PlayerBase::DashAction(const FInputActionValue& Value)
{
	if(DashDisabledTime > 0 || CurrentDashCount >= MaxDashCount || bIsAttackHitCheckOnGoing)
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
	
	ClearState();
	
	CurrentDashCount++;
	DashElapsedTime = 0;
	DashDisabledTime = 0;

	GetCapsuleComponent()->SetCollisionProfileName(CP_EVADE);
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
	Attack();
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
	
	SelectedInteractiveObjectNum++;
	if(SelectedInteractiveObjectNum >= InteractiveObjects.Num())
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

void ALLL_PlayerBase::CharacterRotateToCursor()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	if (bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		MouseWorldLocation,
		MouseWorldLocation + MouseWorldDirection * 10000.f,
		ECC_Visibility
	))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
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

void ALLL_PlayerBase::SetAttackComboCheckState(bool Value)
{
	bCheckAttackComboActionInput = Value;
 	if(!bCheckAttackComboActionInput)
	{
 		ClearState();
		PlayerWeaponComponent->StopMeleeWeaponHitCheck();
	}
}

void ALLL_PlayerBase::SetAttackHitCheckState(bool Value)
{
	bIsAttackHitCheckOnGoing = Value;
	if(bIsAttackHitCheckOnGoing)
	{
		PlayerWeaponComponent->StartMeleeWeaponHitCheck(CurrentComboActionCount);
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("근접 공격 피격 체크 시작")));
			}
		}
#endif
	}
	else
	{
		PlayerWeaponComponent->StopMeleeWeaponHitCheck();
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if(UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
		{
			if(ProtoGameInstance->CheckPlayerDashDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("근접 공격 피격 체크 종료")));
			}
		}
#endif
	}
}

void ALLL_PlayerBase::AttackSequence()
{
	bIsAttackActionOnGoing = true;
	RootComponent->ComponentVelocity = FVector::ZeroVector;
	CharacterAnimInstance->Montage_Play(CharacterDataAsset->AttackAnimMontage);
	CharacterAnimInstance->Montage_JumpToSection(*FString(SECTION_ATTACK).Append(FString::FromInt(CurrentComboActionCount)));
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
		GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);
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

void ALLL_PlayerBase::ClearState()
{
	bIsAttackActionOnGoing = false;
	bIsAttackHitCheckOnGoing = false;
	bCheckAttackComboActionInput = false;
	bIsInvincibleOnDashing = false;

	SetAttackHitCheckState(false);
}

void ALLL_PlayerBase::Dead()
{
	Super::Dead();
	
	DisableInput(Cast<APlayerController>(GetController()));
}

void ALLL_PlayerBase::DeadMontageEndEvent()
{
	// Super::DeadMontageEndEvent();
	PlayerUIManager->TogglePauseWidget(bIsDead);
}

void ALLL_PlayerBase::Attack()
{
	Super::Attack();

	if(bIsAttackActionOnGoing)
	{
		if(bIsAttackHitCheckOnGoing || !bCheckAttackComboActionInput)
		{
			return;
		}
		
		CurrentComboActionCount++;
		if(CurrentComboActionCount >= MaxComboActionCount)
		{
			CurrentComboActionCount = 0;
		}
		bCheckAttackComboActionInput = false;
	}
	else
	{
		CurrentComboActionCount = 0;
	}
	CharacterRotateToCursor();
	AttackSequence();
}
