// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "AnimNotifyState_TimedNiagaraEffect.h"
#include "BrainComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"
#include "GameplayAbilitiesModule.h"
#include "GameplayAbilitySpec.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_AnimMontageSlotName.h"
#include "Constant/LLL_AttributeInitializeGroupName.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "Constant/LLL_MeshSocketName.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerAnimInstance.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/Base/LLL_InteractiveObject.h"
#include "Entity/Object/Thrown/LLL_PlayerChaseHand.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Enumeration/LLL_AbilitySystemEnumHelper.h"
#include "GAS/ASC/LLL_PlayerASC.h"
#include "GAS/Attribute/Character/Player/LLL_AbnormalStatusAttributeSet.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerSkillAttributeSet.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "UI/Entity/Character/Player/LLL_PlayerChaseActionWidget.h"
#include "System/ObjectPooling/LLL_ObjectPoolingComponent.h"
#include "UI/Entity/Character/Player/LLL_PlayerStatusWidget.h"

ALLL_PlayerBase::ALLL_PlayerBase()
{
	ASC = CreateDefaultSubobject<ULLL_PlayerASC>(TEXT("PlayerASC"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	GoldComponent = CreateDefaultSubobject<ULLL_PlayerGoldComponent>(TEXT("PlayerGoldComponent"));
	ObjectPoolingComponent = CreateDefaultSubobject<ULLL_ObjectPoolingComponent>(TEXT("ObjectPoolingComponent"));
	CharacterUIManager = CreateDefaultSubobject<ULLL_PlayerUIManager>(TEXT("PlayerUIManageComponent"));
	
	PlayerCharacterAttributeSet = CreateDefaultSubobject<ULLL_PlayerCharacterAttributeSet>(TEXT("PlayerAttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<ULLL_PlayerSkillAttributeSet>(TEXT("SkillAttributeSet"));
	AbnormalStatusAttributeSet = CreateDefaultSubobject<ULLL_AbnormalStatusAttributeSet>(TEXT("AbnormalStatusAttributeSet"));
	
	ChaseActionGaugeWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ChaseActionGaugeWidgetComponent"));

	ChaseActionGaugeWidgetComponent->SetupAttachment(RootComponent);

	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerBaseDataAsset>(PATH_PLAYER_DATA, EAssertionLevel::Check);
	CameraDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_CameraDataAsset>(PATH_CAMERA_DATA, EAssertionLevel::Check);

	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);

	GetCharacterMovement()->MaxFlySpeed = 10000.f;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(CP_PLAYER);

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	
	SpringArm->TargetArmLength = 0.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	SpringArm->SetUsingAbsoluteRotation(true);
	// SpringArm->SetupAttachment(RootComponent);

	LastCheckedMouseLocation = FVector::Zero();
	
	bIsLowHP = false;
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
		Camera->SetProjectionMode(CameraDataAsset->ProjectionType);
		
		if (Camera->ProjectionMode == ECameraProjectionMode::Orthographic)
		{
			Camera->SetOrthoWidth(CameraDataAsset->CameraDistance);
			Camera->SetUpdateOrthoPlanes(CameraDataAsset->bUseUpdateOrthoPlanes);
			if (CameraDataAsset->bUseConstraintAspectRatio)
			{
				Camera->SetConstraintAspectRatio(CameraDataAsset->bUseConstraintAspectRatio);
				Camera->SetAspectRatio(CameraDataAsset->AspectRatio);
			}
			
			if (CameraDataAsset->bUseAutoCalculate)
			{
				Camera->SetAutoCalculateOrthoPlanes(true);
				Camera->SetAutoPlaneShift(CameraDataAsset->AutoPlaneShift);
			}
			else
			{
				Camera->SetAutoCalculateOrthoPlanes(false);
				Camera->SetOrthoNearClipPlane(CameraDataAsset->OrthographicNearClipDistance);
				Camera->SetOrthoFarClipPlane(CameraDataAsset->OrthographicFarClipDistance);
			}
		}
		else
		{
			SpringArm->TargetArmLength = CameraDataAsset->SpringArmDistance;
			Camera->SetFieldOfView(CameraDataAsset->CameraFOV);
		}
		
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
	GetMesh()->SetCustomDepthStencilValue(2);
	
	ULLL_PlayerChaseActionWidget* ChaseActionWidget = PlayerUIManager->GetChaseActionWidget();
	ChaseActionGaugeWidgetComponent->SetWidget(ChaseActionWidget);
	ChaseActionGaugeWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	ChaseActionGaugeWidgetComponent->SetRelativeLocation(PlayerDataAsset->ChaseActionGaugeLocation);
	ChaseActionGaugeWidgetComponent->SetDrawSize(PlayerDataAsset->ChaseActionGaugeSize);
	ChaseActionGaugeWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChaseActionGaugeWidgetComponent->SetTickWhenOffscreen(true);
	ChaseActionWidget->SetCircleProgressBarValue(1.0f);
}

void ALLL_PlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MoveCameraToMouseCursor();
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerCollisionDebug())
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

void ALLL_PlayerBase::InitAttributeSet()
{
	Super::InitAttributeSet();

	// DefaultGame.ini의 [/Script/GameplayAbilities.AbilitySystemGlobals] 항목에 테이블 미리 추가해놔야 정상 작동함.
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(ASC, ATTRIBUTE_INIT_PLAYER, Level, true);
}

void ALLL_PlayerBase::SetFModParameter(EFModParameter FModParameter)
{
	Super::SetFModParameter(FModParameter);

	if (FModParameter == EFModParameter::PlayerDamagedTypeParameter)
	{
		for (auto DamagedEventParameterProperty : PlayerDataAsset->DamagedEventParameterProperties)
		{
			if (LastAttackerMonsterId != DamagedEventParameterProperty.Key)
			{
				continue;
			}

			SetParameter(FModParameter, static_cast<float>(DamagedEventParameterProperty.Value));
		}
	}
	else if (FModParameter == EFModParameter::PlayerWalkMaterialParameter)
	{
		const TEnumAsByte<EPhysicalSurface> SurfaceType = PlayerAnimInstance->GetSurfaceType();
		for (auto StepEventParameterProperty : PlayerDataAsset->StepEventParameterProperties)
		{
			if (SurfaceType != StepEventParameterProperty.Key)
			{
				continue;
			}

			SetParameter(FModParameter, static_cast<float>(StepEventParameterProperty.Value));
		}
	}
	else if (FModParameter == EFModParameter::PlayerAttackCountParameter || FModParameter == EFModParameter::PlayerAttackHitCountParameter)
	{
		SetParameter(FModParameter, CurrentCombo - 1);
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

FVector ALLL_PlayerBase::CheckMouseLocation()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector MouseWorldLocation;
	FVector MouseWorldDirection;
	PlayerController->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection);
	
	FHitResult HitResult;
	const FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		MouseWorldLocation,
		MouseWorldLocation + MouseWorldDirection * 10000.f,
		ECC_TRACE_FIELD
	);
	
	if (!bResult)
	{
		return FVector::Zero();
	}
	
	LastCheckedMouseLocation = HitResult.ImpactPoint;
	
	HitResult.Init();
	const float CorrectionCheckRadius = PlayerCharacterAttributeSet->GetTargetingCorrectionRadius();
	bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		LastCheckedMouseLocation,
		LastCheckedMouseLocation,
		FQuat::Identity,
		ECC_ENEMY_HIT,
		FCollisionShape::MakeSphere(CorrectionCheckRadius),
		Params
		);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckPlayerAttackDebug() || DebugGameInstance->CheckPlayerSkillDebug() || DebugGameInstance->CheckPlayerChaseActionDebug())
		{
			DrawDebugLine(GetWorld(), MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000.f, FColor::Red, false, 3.f);
			DrawDebugPoint(GetWorld(), LastCheckedMouseLocation, 10.f, FColor::Red, false, 3.f);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("마우스 월드 좌표: %f, %f, %f"), LastCheckedMouseLocation.X, LastCheckedMouseLocation.Y, LastCheckedMouseLocation.Z));
			if(bResult)
			{
				DrawDebugSphere(GetWorld(), LastCheckedMouseLocation, CorrectionCheckRadius, 16, FColor::Green, false, 2.f);
			}
			else
			{
				DrawDebugSphere(GetWorld(), LastCheckedMouseLocation, CorrectionCheckRadius, 16, FColor::Red, false, 2.f);
			}
		}
	}
#endif
	
	if (bResult)
	{
		LastCheckedMouseLocation = HitResult.GetActor()->GetActorLocation();
	}
	
	return LastCheckedMouseLocation;
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
	
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_GAS_PLAYER_STATE_CHASE_PROGRESS))
	{
		return;
	}

	AddMovementInput(MoveDirection, 1.f);
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if(DebugGameInstance->CheckPlayerMovementDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("이동 입력 방향: %f, %f"), MoveDirection.X, MoveDirection.Y));
		}
	}
#endif
}

void ALLL_PlayerBase::DashAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	const int32 InputID = static_cast<int32>(InputName);
	if (FGameplayAbilitySpec* DashSpec = ASC->FindAbilitySpecFromInputID(InputID))
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
	if (CastChecked<ULLL_GameInstance>(GetGameInstance())->CheckCustomTimeDilationIsChanging())
	{
		return;
	}
	
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

void ALLL_PlayerBase::PlayerRotateToMouseCursor(float RotationMultiplyValue, bool UseLastLocation)
{
	FVector MouseWorldLocation;
	if (UseLastLocation)
	{
		MouseWorldLocation = LastCheckedMouseLocation;
	}
	else
	{
		MouseWorldLocation = CheckMouseLocation();
	}
	
	FVector ViewDirection = (MouseWorldLocation - GetActorLocation()).GetSafeNormal();
	ViewDirection.Z = 0.f;
	MouseDirectionRotator = ViewDirection.Rotation();
	ToCursorRotationMultiplyValue = RotationMultiplyValue;
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::TurnToMouseCursor);
}

void ALLL_PlayerBase::TurnToMouseCursor()
{
	if (GetActorRotation() == MouseDirectionRotator || !GetCharacterAnimInstance()->IsSlotActive(ANIM_SLOT_ATTACK))
	{
		return;
	}
	
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), MouseDirectionRotator, GetWorld()->GetDeltaSeconds(), PlayerCharacterAttributeSet->GetTurnSpeed() * ToCursorRotationMultiplyValue));
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::TurnToMouseCursor);
}

void ALLL_PlayerBase::MoveCameraToMouseCursor()
{
	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector2d MouseScreenLocation;
	FVector2d ScreenViewport;
	int32 ViewportX;
	int32 ViewportY;
	PlayerController->GetMousePosition(MouseScreenLocation.X, MouseScreenLocation.Y);
	PlayerController->GetViewportSize(ViewportX, ViewportY);
	ScreenViewport.X = ViewportX;
	ScreenViewport.Y = ViewportY;

	const FVector2d MovementDirection = (MouseScreenLocation / ScreenViewport - FVector2d(0.5f, 0.5f)) * FVector2d(1.f, -1.f);
	FVector CameraMoveVector = FVector(MovementDirection.X, MovementDirection.Y, 0.f);
	CameraMoveVector = SpringArm->GetDesiredRotation().UnrotateVector(CameraMoveVector);
	
	CameraMoveVector *= 500.f;
	if (CameraMoveVector.ContainsNaN())
	{
		return;
	}
	
	SpringArm->SetRelativeLocation(FVector(CameraMoveVector.Y, CameraMoveVector.X, 0.f) + GetActorLocation());
}

void ALLL_PlayerBase::SetParameter(EFModParameter FModParameter, float value) const
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

void ALLL_PlayerBase::Damaged(AActor* Attacker, bool IsDOT)
{
	Super::Damaged(Attacker, IsDOT);
	
	if (IsValid(PlayerDataAsset->DamagedAnimMontage) && !IsDOT)
	{
		PlayerAnimInstance->Montage_Play(PlayerDataAsset->DamagedAnimMontage);
	}

	ActivatePPLowHP();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::DeactivatePPLowHP);
	if (PlayerCharacterAttributeSet->GetCurrentHealth() / PlayerCharacterAttributeSet->GetMaxHealth() <= 0.3f)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::PlayLowHPAnimation);
	}

	const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(Attacker);
	if (!IsValid(Monster))
	{
		return;
	}

	LastAttackerMonsterId = Monster->GetId();
}

void ALLL_PlayerBase::Dead()
{
	Super::Dead();

	// TODO: 목숨 같은거 생기면 사이에 추가하기
	
	DisableInput(Cast<APlayerController>(GetController()));
	
	PlayerAnimInstance->StopAllMontages(1.f);
	if (IsValid(PlayerDataAsset->DeadAnimMontage))
	{
		PlayerAnimInstance->Montage_Play(PlayerDataAsset->DeadAnimMontage);
	}

	ALevelSequenceActor* DeadSequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;
	Settings.bHideHud = false;
	Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceRestoreState;
	if (!IsValid(DeadSequenceActor) || !PlayerDataAsset->DeadSequencer)
	{
		return;
	}
	GoldComponent->SetGoldWidgetVisibility(false);
	PlayerUIManager->SetAllWidgetVisibility(false);
	Camera->SetUpdateOrthoPlanes(true);
	DeadSequenceActor->PlaybackSettings = Settings;
	DeadSequenceActor->SetSequence(PlayerDataAsset->DeadSequencer);
	DeadSequenceActor->InitializePlayer();

	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	
	GetWorld()->SweepMultiByProfile(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		CP_BULLET_TIME_INFLUENCED,
		FCollisionShape::MakeBox(FVector(10000.f, 10000.f, 1000.f)),
		Params
		);

	if (!HitResults.IsEmpty())
	{
		for (auto HitResult : HitResults)
		{
			if (ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(HitResult.GetActor()))
			{
				Cast<AAIController>(Monster->GetController())->GetBrainComponent()->StopLogic(TEXT("PlayerDead"));
				Monster->StopAnimMontage();
			}
			HitResult.GetActor()->CustomTimeDilation = 0.01f;
			HitResult.GetActor()->SetHidden(true);
		}
	}
	
	FTransform DissolveStartTransform = GetMesh()->GetSocketTransform(SOCKET_OVERHEAD);
	DeadSequenceDissolveActor = GetWorld()->SpawnActor<AActor>(PlayerDataAsset->DeadSequenceDissolveActor, DissolveStartTransform);
	DeadSequenceActor->FinishSpawning(FTransform::Identity);

	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::DropDissolveActor);
}

void ALLL_PlayerBase::DropDissolveActor()
{
	if (DeadSequenceDissolveActor->GetActorLocation().Z < GetMesh()->GetComponentLocation().Z - 100.f)
	{
		return;
	}
	DeadSequenceDissolveActor->SetActorLocation(DeadSequenceDissolveActor->GetActorLocation() - FVector(0.f, 0.f, PlayerDataAsset->DissolveActorFallSpeed));
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::DropDissolveActor);
}

void ALLL_PlayerBase::DeadMotionEndedHandle()
{
	PlayerUIManager->SetAllWidgetVisibility(true);
	PlayerUIManager->TogglePauseWidget(bIsDead);
}

void ALLL_PlayerBase::DeactivatePPLowHP()
{
	const ULLL_GameInstance* GameInstance = Cast<ULLL_GameInstance>(GetGameInstance());
	const UMaterialParameterCollection* MPC = GameInstance->GetPostProcessMPC();
	ScalarValue += GetWorld()->GetDeltaSeconds() / 3;
	GetWorld()->GetParameterCollectionInstance(MPC)->SetScalarParameterValue(PP_PLAYER_LOWHP_RADIUS, ScalarValue);
	if (ScalarValue <= PlayerDataAsset->HPLowScalarMaxValue)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ALLL_PlayerBase::DeactivatePPLowHP);
	}
}

void ALLL_PlayerBase::ActivatePPLowHP()
{
	const ULLL_GameInstance* GameInstance = Cast<ULLL_GameInstance>(GetGameInstance());
	const UMaterialParameterCollection* MPC = GameInstance->GetPostProcessMPC();
	ScalarValue = PlayerDataAsset->HPLowScalarLowValue;
	GetWorld()->GetParameterCollectionInstance(MPC)->SetScalarParameterValue(PP_PLAYER_LOWHP_RADIUS, ScalarValue);
}

void ALLL_PlayerBase::PlayLowHPAnimation()
{
	ULLL_PlayerStatusWidget* StateWidget = Cast<ULLL_PlayerStatusWidget>(PlayerUIManager->GetCharacterStatusWidget());
	StateWidget->PlayLowHPAnimation();
	FTimerHandle LowHPHandle;
	if (PlayerCharacterAttributeSet->GetCurrentHealth() / PlayerCharacterAttributeSet->GetMaxHealth() <= 0.3f && !bIsLowHP)
	{
		bIsLowHP = true;
		GetWorld()->GetTimerManager().SetTimer(LowHPHandle, this, &ALLL_PlayerBase::PlayLowHPAnimation, 1.0f, true);
	}
	else if (PlayerCharacterAttributeSet->GetCurrentHealth() / PlayerCharacterAttributeSet->GetMaxHealth() > 0.3f)
	{
		GetWorld()->GetTimerManager().ClearTimer(LowHPHandle);
	}
}
