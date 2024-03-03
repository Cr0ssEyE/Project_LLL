// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbilitySpec.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_PlayerBaseDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Entity/Object/Interactive/LLL_InteractiveObject.h"
#include "Entity/Object/Thrown/LLL_PlayerWireHand.h"
#include "Enumeration/LLL_AbilityKeyHelper.h"
#include "Game/ProtoGameInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/Attribute/Player/LLL_PlayerAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLLConstructorHelper.h"

ALLL_PlayerBase::ALLL_PlayerBase()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	PlayerUIManager = CreateDefaultSubobject<ULLL_PlayerUIManager>(TEXT("PlayerUIManageComponent"));
	PlayerAttributeSet = CreateDefaultSubobject<ULLL_PlayerAttributeSet>(TEXT("PlayerAttributes"));
	
	WireHandActorComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("WireHandComponent"));
	TSubclassOf<ALLL_PlayerWireHand> WireHand = ALLL_PlayerWireHand::StaticClass();
	WireHandActorComponent->SetChildActorClass(WireHand);
	WireHandActorComponent->SetupAttachment(RootComponent);
	
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_PlayerBaseDataAsset>(PATH_PLAYER_DATA, EAssertionLevel::Check);
	PlayerDataAsset = Cast<ULLL_PlayerBaseDataAsset>(CharacterDataAsset);
	if (IsValid(CharacterDataAsset))
	{
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
	}
}

void ALLL_PlayerBase::BeginPlay()
{
	Super::BeginPlay();

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
		// GE 기반으로 자신의 어트리뷰트 초기화
		ASC->AddSpawnedAttribute(PlayerAttributeSet);
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
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
	
	EnhancedInputComponent->BindAction(PlayerDataAsset->MoveInputAction, ETriggerEvent::Triggered, this, &ALLL_PlayerBase::MoveAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->AttackInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::AttackAction, EAbilityInputName::Attack);
	EnhancedInputComponent->BindAction(PlayerDataAsset->DashInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::DashAction, EAbilityInputName::Dash);
	EnhancedInputComponent->BindAction(PlayerDataAsset->SkillInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::SkillAction, EAbilityInputName::Skill);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractionInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InteractiveTargetChangeInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InteractiveTargetChangeAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->InventoryInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::InventoryAction);
	EnhancedInputComponent->BindAction(PlayerDataAsset->PauseInputAction, ETriggerEvent::Started, this, &ALLL_PlayerBase::PauseAction);
}

void ALLL_PlayerBase::AddInteractableObject(ALLL_InteractiveObject* Object)
{
	InteractiveObjects.Emplace(Object);
	PlayerUIManager->EnableInteractionWidget();
	if (InteractiveObjects.Num() == 1)
	{
		SelectedInteractiveObjectNum = 0;
	}
	PlayerUIManager->UpdateInteractionWidget(InteractiveObjects[SelectedInteractiveObjectNum], InteractiveObjects.Num() - 1);
}

void ALLL_PlayerBase::RemoveInteractableObject(ALLL_InteractiveObject* RemoveObject)
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
			if (ProtoGameInstance->CheckPlayerAttackDebug() || ProtoGameInstance->CheckPlayerSkillDebug())
			{
				DrawDebugLine(GetWorld(), MouseWorldLocation, MouseWorldLocation + MouseWorldDirection * 10000.f, FColor::Red, false, 3.f);
				DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.f, FColor::Red, false, 3.f);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("마우스 월드 좌표: %f, %f, %f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z));
			}
		}
#endif
		
		FVector TrueMouseWorldLocation = HitResult.ImpactPoint;
		return TrueMouseWorldLocation;
	}
	return FVector::Zero();
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

void ALLL_PlayerBase::DashAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	int32 InputID = static_cast<int32>(InputName);
	FGameplayAbilitySpec* DashSpec = ASC->FindAbilitySpecFromInputID(InputID);
	if(DashSpec)
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

	int32 InputID = static_cast<int32>(InputName);
	FGameplayAbilitySpec* AttackSpec = ASC->FindAbilitySpecFromInputID(InputID);
	if(AttackSpec)
	{
		CharacterRotateToCursor();
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
	//Attack();
}

void ALLL_PlayerBase::SkillAction(const FInputActionValue& Value, EAbilityInputName InputName)
{
	int32 InputID = static_cast<int32>(InputName);
	FGameplayAbilitySpec* SkillSpec = ASC->FindAbilitySpecFromInputID(InputID);
	if(SkillSpec)
	{
		CharacterRotateToCursor();
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

void ALLL_PlayerBase::CharacterRotateToCursor()
{
	FVector MouseWorldLocation = GetMouseLocation();
	FVector ViewDirection = (MouseWorldLocation - GetActorLocation()).GetSafeNormal();
	ViewDirection.Z = 0.f;
	SetActorRotation(ViewDirection.Rotation());
}

void ALLL_PlayerBase::Dead()
{
	Super::Dead();

	// TODO: 목숨 같은거 생기면 사이에 추가하기
	
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

}
