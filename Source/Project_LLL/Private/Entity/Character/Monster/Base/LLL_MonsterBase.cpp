// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BrainComponent.h"
#include "GameplayAbilitiesModule.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_AttributeInitializeGroupName.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_GraphicParameterNames.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "DataAsset/Global/LLL_GlobalParameterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"
#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrength.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MGA_Charge.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/ASC/LLL_MonsterASC.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "Navigation/PathFollowingComponent.h"
#include "System/MapGimmick/LLL_FallableWallGimmick.h"
#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Util/LLL_MathHelper.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	ASC = CreateDefaultSubobject<ULLL_MonsterASC>(TEXT("MonsterASC"));
	MonsterAttributeSet = CreateDefaultSubobject<ULLL_MonsterAttributeSet>(TEXT("MonsterAttributeSet"));
	CharacterUIManager = CreateDefaultSubobject<ULLL_MonsterBaseUIManager>(TEXT("MonsterUIManageComponent"));
	MonsterStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterStatusWidgetComponent"));
	
	MonsterStatusWidgetComponent->SetupAttachment(RootComponent);

	GetMesh()->SetCollisionProfileName(CP_MONSTER);
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ALLL_MonsterBaseAIController::StaticClass();

	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	DropGoldEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'"), EAssertionLevel::Check);

	MaskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask"));
	MaskMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	MaskMeshComponent->SetupAttachment(RootComponent);

	StackVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StackStatusEffect"));
	StackVFXComponent->SetupAttachment(RootComponent);
	StackVFXComponent->SetAutoActivate(false);

	BleedingVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BleedingStatusEffect"));
	BleedingVFXComponent->SetupAttachment(RootComponent);
	BleedingVFXComponent->SetAutoActivate(false);

	AttributeInitId = ATTRIBUTE_INIT_MONSTER;
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(DropGoldEffect, 1.0, EffectContextHandle);
	if(EffectSpecHandle.IsValid())
	{
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
	
	MonsterStatusWidgetComponent->SetWidget(CharacterUIManager->GetCharacterStatusWidget());
	MonsterStatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	MonsterStatusWidgetComponent->SetRelativeLocation(MonsterBaseDataAsset->StatusGaugeLocation);
	MonsterStatusWidgetComponent->SetDrawSize(MonsterBaseDataAsset->StatusGaugeSize);
	MonsterStatusWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MonsterStatusWidgetComponent->SetTickWhenOffscreen(true);

	MaskMeshComponent->SetStaticMesh(MonsterBaseDataAsset->MaskMesh);
	MaskMeshComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, MonsterBaseDataAsset->MaskAttachSocketName);
	MaskMeshComponent->SetRelativeTransform(MonsterBaseDataAsset->MaskTransform);

	UNiagaraSystem* StackCountNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->StackCountNiagaraSystem;
	if (IsValid(StackCountNiagaraSystem))
	{
		StackVFXComponent->SetAsset(StackCountNiagaraSystem);
		StackVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_OVERHEAD);
	}

	UNiagaraSystem* BleedingNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->BleedingNiagaraSystem;
	if (IsValid(BleedingNiagaraSystem))
	{
		BleedingVFXComponent->SetAsset(BleedingNiagaraSystem);
		BleedingVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_CHEST);
	}
	
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	MonsterBaseAIController->StopLogic("Before Initialize");
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		DebugGameInstance->MonsterToggleAIDelegate.AddDynamic(this, &ALLL_MonsterBase::ToggleAIHandle);
		DebugGameInstance->MonsterToggleAIDelegate.Broadcast(DebugGameInstance->GetMonsterToggleAIDebug());
	}
#endif
}

void ALLL_MonsterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsKnockBacking)
	{
		FVector VelocityWithKnockBack = GetVelocity();
		
		// 넉백 끝났을때 처리
		bool IsMoving = CastChecked<ALLL_MonsterBaseAIController>(GetController())->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving;
		if ((VelocityWithKnockBack == FVector::ZeroVector || IsMoving) && StartKnockBackVelocity)
		{
			CustomTimeDilation = 1.f;
			UE_LOG(LogTemp, Log, TEXT("%s가 넉백 끝"), *GetName())
			bIsKnockBacking = false;
			StartKnockBackVelocity = false;
			DeflectCount = 0;
			KnockBackSender = nullptr;
			KnockBackTargetDamaged = false;
			KnockBackCauserDamaged = false;
			const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
			if (!CheckCharacterIsDead())
			{
				MonsterBaseAIController->StartLogic();
			}
			CastChecked<ALLL_MonsterBaseAIController>(GetController())->ResumeMove(FAIRequestID::AnyRequest);
		}
		else
		{
			StartKnockBackVelocity = true;
		}
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterCollisionDebug())
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

void ALLL_MonsterBase::InitAttributeSet()
{
	Super::InitAttributeSet();

	const int32 Data = Id * 100 + Level;
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(ASC, AttributeInitId, Data, true);

	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	MonsterBaseAIController->StartLogic();
}

void ALLL_MonsterBase::SetFModParameter(EFModParameter FModParameter)
{
	Super::SetFModParameter(FModParameter);

	if (FModParameter == EFModParameter::MonsterWalkMaterialParameter)
	{
		const TEnumAsByte<EPhysicalSurface> SurfaceType = GetCharacterAnimInstance()->GetSurfaceType();
		for (auto StepEventParameterProperty : MonsterBaseDataAsset->StepEventParameterProperties)
		{
			if (SurfaceType != StepEventParameterProperty.Key)
			{
				continue;
			}

			SetOnceParameterByTupleValue(FModParameter, static_cast<float>(StepEventParameterProperty.Value));
		}
	}
}

void ALLL_MonsterBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// 몬스터가 날아가다 부딪혔을때 처리
	UCharacterMovementComponent* CharacterMovementComponent = CastChecked<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterMovementComponent->MovementMode == MOVE_Flying)
	{
		CharacterMovementComponent->MovementMode = MOVE_Walking;
		GetMesh()->SetCollisionProfileName(CP_MONSTER);
		GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);

		const ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(GetOwner());
		const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Other);
		if (IsValid(ManOfStrength) && IsValid(Player))
		{
			const ULLL_ManOfStrengthDataAsset* ManOfStrengthDataAsset = CastChecked<ULLL_ManOfStrengthDataAsset>(ManOfStrength->GetCharacterDataAsset());
			const ULLL_MonsterAttributeSet* OwnerMonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrength->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
			const float OffencePower = OwnerMonsterAttributeSet->GetManOfStrengthThrowOffencePower();
			
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ManOfStrengthDataAsset->ThrowDamageEffect, ManOfStrength->GetAbilityLevel(), EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *Other->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			}
		}
		
		SetOwner(nullptr);
		if (!CheckCharacterIsDead())
		{
			CastChecked<ALLL_MonsterBaseAIController>(GetController())->StartLogic();
		}
	}

	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (bIsKnockBacking && IsValid(Player))
	{
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		if (!Cast<ALLL_BaseCharacter>(Other) && !Cast<ALLL_FallableWallGimmick>(Other))
		{
			float DotProduct = FVector::DotProduct(HitNormal, FVector::UpVector);
			float AngleInRadians = FMath::Acos(DotProduct);
			float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);
			if (AngleInDegrees > 45.0f)
			{
				DamageKnockBackCauser(Player);

				// 리바운드 이누리아
				if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_DEFLECT_BY_WALL) && DeflectCount < Player->GetDeflectCount())
				{
					// 반사 벡터 구하기
					LastKnockBackVelocity = LastKnockBackVelocity - 2 * FVector::DotProduct(LastKnockBackVelocity, HitNormal) * HitNormal;
					DeflectCount++;
					UE_LOG(LogTemp, Log, TEXT("벽에 %d번 튕김"), DeflectCount)
					AddKnockBackVelocity(LastKnockBackVelocity, LastKnockBackPower);
					StartKnockBackVelocity = false;
				}
			}
		}

		ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(Other);
		if (IsValid(OtherMonster))
		{
			DamageKnockBackTarget(Player, OtherMonster);
			DamageKnockBackCauser(Player);

			// 연쇄 작용 이누리아
			if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_KNOCK_BACK_TRANSMISSION) && (!IsValid(KnockBackSender) || OtherMonster != KnockBackSender) && !OtherMonster->IsKnockBacking())
			{
				FVector Direction = (OtherMonster->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
				const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerASC->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
				float KnockBackPower = Player->GetKnockBackTransmissionKnockBackPower();
				KnockBackPower *= PlayerAttributeSet->GetKnockBackPowerRate();
				KnockBackPower += PlayerAttributeSet->GetKnockBackPowerPlus();
				UE_LOG(LogTemp, Log, TEXT("연쇄 작용으로 %s에게 %f만큼 넉백"), *Other->GetName(), KnockBackPower)
			
				FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(Direction, KnockBackPower);
				OtherMonster->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
			
				const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
				float OffencePower = Player->GetKnockBackTransmissionOffencePower();
				OffencePower *= PlayerAttributeSet->GetAllOffencePowerRate();
				OffencePower += PlayerAttributeSet->GetAllOffencePowerPlus();
			
				FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
				EffectContextHandle.AddSourceObject(this);
				const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->KnockBackTransmissionDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
				if (EffectSpecHandle.IsValid())
				{
					EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
					UE_LOG(LogTemp, Log, TEXT("연쇄 작용으로 %s에게 %f만큼 데미지"), *Other->GetName(), OffencePower)
					ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, OtherMonster->GetAbilitySystemComponent());
				}
			}
		}
	}
}

void ALLL_MonsterBase::Charge()
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_CHARGE)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 차지 수행"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_MonsterBase::Damaged(AActor* Attacker, bool IsDOT)
{
	Super::Damaged(Attacker, IsDOT);
	
	ShowHitEffect();
	RecognizePlayerToAroundMonster();

	if (Cast<ALLL_BossMonster>(this))
	{
		return;
	}

	if (bIsAttacking)
	{
		return;
	}

	ULLL_BaseCharacterAnimInstance* AnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(AnimInstance) || IsDOT)
	{
		return;
	}

	AnimInstance->StopAllMontages(1.0f);
	if (GetAbilitySystemComponent()->HasMatchingGameplayTag(TAG_GAS_STATE_COLLIDE_OTHER) && IsValid(MonsterBaseDataAsset->KnockBackCollideMontage))
	{
		FVector HitDirection = (GetActorLocation() - GetLastCollideLocation()).GetSafeNormal2D();
		HitDirection.Z = 0.f;
		SetActorRotation(HitDirection.Rotation(), ETeleportType::TeleportPhysics);
		
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("충돌 경직")));
		PlayAnimMontage(MonsterBaseDataAsset->KnockBackCollideMontage);
	}
	else if (IsValid(MonsterBaseDataAsset->DamagedAnimMontage))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("일반 경직")));
		PlayAnimMontage(MonsterBaseDataAsset->DamagedAnimMontage);
	}

	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
		TempNiagaraComponent->DestroyComponent();
		NiagaraComponents.Remove(TempNiagaraComponent);
	}

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterCollisionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 경직"), *GetName()));
		}
	}
#endif
}

void ALLL_MonsterBase::Dead()
{
	Super::Dead();

	CharacterAnimInstance->StopAllMontages(1.0f);

	GetCapsuleComponent()->SetCollisionProfileName(CP_RAGDOLL);
	GetMesh()->SetCollisionProfileName(CP_RAGDOLL);
	GetMesh()->SetCustomDepthStencilValue(0);
	
	DropGold(TAG_GAS_SYSTEM_DROP_GOLD, 0);

	ULLL_BaseCharacterAnimInstance* AnimInstance = CastChecked<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->StopAllMontages(1.0f);
	
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	MonsterBaseAIController->StopLogic("Monster Is Dead");

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (IsValid(PlayerBase))
	{
		FVector ImpulseDirection = GetActorLocation() - PlayerBase->GetActorLocation();
		ImpulseDirection.Normalize();
		const ULLL_PlayerCharacterAttributeSet* PlayerAttributeSet = CastChecked<ULLL_PlayerCharacterAttributeSet>(PlayerBase->GetAbilitySystemComponent()->GetAttributeSet(ULLL_PlayerCharacterAttributeSet::StaticClass()));
		const float ImpulseStrength = PlayerAttributeSet->GetImpulseStrength();
		const FVector FinalImpulse = ImpulseDirection * ImpulseStrength * 100.0f;
		
		GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse);
	}

	MonsterStatusWidgetComponent->SetHiddenInGame(true);

	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
    	TempNiagaraComponent->DestroyComponent();
		NiagaraComponents.Remove(TempNiagaraComponent);
	}
	BleedingVFXComponent->SetHiddenInGame(true);
	StackVFXComponent->SetHiddenInGame(true);
	
	RecognizePlayerToAroundMonster();

	const float DestroyTimer = MonsterAttributeSet->GetDestroyTimer();
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		Destroy();
	}), DestroyTimer, false);
}

void ALLL_MonsterBase::AddKnockBackVelocity(FVector& KnockBackVelocity, float KnockBackPower)
{
	CastChecked<ALLL_MonsterBaseAIController>(GetController())->PauseMove(FAIRequestID::AnyRequest);
	
	const float DecreaseVelocityByWeight = FMath::Max(0.f, (MonsterAttributeSet->GetWeight() - 1) * GetGameInstance<ULLL_GameInstance>()->GetGlobalParametersDataAsset()->DecreaseVelocityPerWeight);
	KnockBackVelocity *= 1 - DecreaseVelocityByWeight;
	KnockBackPower *= 1 - DecreaseVelocityByWeight;
	KnockBackVelocity.Z = 0.f;

	// 낙사 넉백일때 처리
	if (KnockBackPower < 0.f)
	{
		LaunchCharacter(KnockBackVelocity, true, true);
		UE_LOG(LogTemp, Log, TEXT("%s가 낙사 넉백 시작"), *GetName())
		bIsKnockBacking = true;
		LastKnockBackVelocity = KnockBackVelocity;
		LastKnockBackPower = KnockBackPower;
		
		CharacterAnimInstance->StopAllMontages(1.0f);
		const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
		MonsterBaseAIController->StopLogic("Monster Is Fallable");
		return;
	}
	
	ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (IsValid(Player))
	{
		UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		
		// 넘치는 힘 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_FASTER_KNOCK_BACK))
		{
			CustomTimeDilation = 1 + Player->GetFasterKnockBackSpeedRate();
		}
	}
	
	if (FLLL_MathHelper::CheckFallableKnockBackPower(GetWorld(), KnockBackPower) && GetCapsuleComponent()->GetCollisionProfileName() != CP_MONSTER_FALLABLE)
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE);
	}
	
	GetCharacterMovement()->Velocity = FVector::Zero();
	LaunchCharacter(KnockBackVelocity, true, true);
	UE_LOG(LogTemp, Log, TEXT("%s가 %f 파워로 넉백 시작"), *GetName(), KnockBackPower)
	bIsKnockBacking = true;
	LastKnockBackVelocity = KnockBackVelocity;
	LastKnockBackPower = KnockBackPower;
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterHitCheckDebug() || DebugGameInstance->CheckMonsterCollisionDebug())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("넉백 방향 및 속도 : %f, %f, %f || %f"), KnockBackVelocity.X, KnockBackVelocity.Y, KnockBackVelocity.Z, KnockBackVelocity.Length()));
		}
	}
#endif
}

float ALLL_MonsterBase::GetChargeTimer() const
{
	return MonsterAttributeSet->GetChargeTimer();
}

void ALLL_MonsterBase::Attack() const
{
	if (ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(TAG_GAS_MONSTER_ATTACK)))
	{
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckMonsterAttackDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%s : 공격 수행"), *GetName()));
			}
		}
#endif
	}
}

void ALLL_MonsterBase::RecognizePlayerToAroundMonster() const
{
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	const float ClusterRecognizeRadius = MonsterAttributeSet->GetClusterRecognizeRadius();
	
	GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_ENEMY,
		FCollisionShape::MakeSphere(ClusterRecognizeRadius),
		Params
		);

	FColor DebugColor = FColor::Red;
	if (!HitResults.IsEmpty())
	{
		for (auto HitResult : HitResults)
		{
			if (const ALLL_MonsterBase* Monster = Cast<ALLL_MonsterBase>(HitResult.GetActor()))
			{
				const ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(Monster->GetController());
				ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));
				if (!IsValid(Player))
				{
					Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
				}
				MonsterAIController->SetPlayer(Player);
				DebugColor = FColor::Green;
			}
		}
	}
	
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (DebugGameInstance->CheckMonsterAttackDebug())
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ClusterRecognizeRadius, 16, DebugColor, false, 2.0f);
		}
	}
#endif
}

void ALLL_MonsterBase::ShowHitEffect()
{
	if (!IsValid(HitEffectOverlayMaterialInstance))
	{
		HitEffectOverlayMaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetOverlayMaterial(), this);
		GetMesh()->SetOverlayMaterial(HitEffectOverlayMaterialInstance);
		for (auto ChildComponent : GetMesh()->GetAttachChildren())
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ChildComponent))
			{
				StaticMeshComponent->SetOverlayMaterial(HitEffectOverlayMaterialInstance);
			}
		}
	}

	HitEffectOverlayMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, 1.f);
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_MonsterBase::UpdateMonsterHitVFX);
}

void ALLL_MonsterBase::ConnectOwnerDeadDelegate()
{
	if (!IsValid(GetOwner()))
	{
		return;
	}
	
	ALLL_BaseCharacter* OwnerCharacter = CastChecked<ALLL_BaseCharacter>(GetOwner());
	if (!OwnerCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_MonsterBase::OwnerCharacterDeadHandle))
	{
		CastChecked<ALLL_BaseCharacter>(Owner)->CharacterDeadDelegate.AddDynamic(this, &ALLL_MonsterBase::OwnerCharacterDeadHandle);
	}
}

void ALLL_MonsterBase::DisconnectOwnerDeadDelegate()
{
	ALLL_BaseCharacter* OwnerCharacter = Cast<ALLL_BaseCharacter>(GetOwner());
	if (IsValid(OwnerCharacter) && OwnerCharacter->CharacterDeadDelegate.IsAlreadyBound(this, &ALLL_MonsterBase::OwnerCharacterDeadHandle))
	{
		CastChecked<ALLL_BaseCharacter>(Owner)->CharacterDeadDelegate.RemoveDynamic(this, &ALLL_MonsterBase::OwnerCharacterDeadHandle);
	}
}

void ALLL_MonsterBase::DamageKnockBackTarget(ALLL_PlayerBase* Player, const ALLL_MonsterBase* Monster)
{
	if (!KnockBackTargetDamaged)
	{
		KnockBackTargetDamaged = true;
		const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
		
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(Player, this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->KnockBackTargetDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Monster->GetAbilitySystemComponent());
		}
	}
}

void ALLL_MonsterBase::DamageKnockBackCauser(ALLL_PlayerBase* Player)
{
	if (!KnockBackCauserDamaged)
	{
		KnockBackCauserDamaged = true;
		const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
		
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(Player, this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->KnockBackCauserDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void ALLL_MonsterBase::ToggleAIHandle(bool value)
{
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterBaseAIController))
	{
		UBrainComponent* BrainComponent = MonsterBaseAIController->GetBrainComponent();
		if (value)
		{
			BrainComponent->ResumeLogic(TEXT("AI Debug Is Activated"));
		}
		else
		{
			BrainComponent->PauseLogic(TEXT("AI Debug Is Deactivated"));
		}
	}
}

void ALLL_MonsterBase::OwnerCharacterDeadHandle(ALLL_BaseCharacter* Character)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	UCharacterMovementComponent* CharacterMovementComponent = CastChecked<UCharacterMovementComponent>(GetMovementComponent());
	CharacterMovementComponent->MovementMode = MOVE_Walking;
	GetMesh()->SetCollisionProfileName(CP_MONSTER);
	GetCapsuleComponent()->SetCollisionProfileName(CP_MONSTER);
	
	SetOwner(nullptr);
	CastChecked<ALLL_MonsterBaseAIController>(GetController())->StartLogic();
}

void ALLL_MonsterBase::UpdateStackVFX(uint8 NewCount, uint8 MaxCount)
{
	if (NewCount > MaxCount)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("표식 값 갱신: %d"), NewCount));

	StackVFXComponent->SetFloatParameter(NS_MARK_COUNT, FMath::Max(NewCount - 1.f, 0.f));
	
	if (NewCount > 0)
	{
		StackVFXComponent->ActivateSystem();
		StackVFXComponent->SetVisibility(true);
	}
	else
	{
		StackVFXComponent->Deactivate();
		StackVFXComponent->SetVisibility(false);
	}
	
}

void ALLL_MonsterBase::UpdateBleedingVFX(bool ActiveState)
{
	if (ActiveState)
	{
		BleedingVFXComponent->ActivateSystem();
	}
	else
	{
		if (GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING)))
		{
			return;
		}
		BleedingVFXComponent->Deactivate();
	}
}

void ALLL_MonsterBase::UpdateMonsterHitVFX()
{
	float CurrentOpacity = HitEffectOverlayMaterialInstance->K2_GetScalarParameterValue(MAT_PARAM_OPACITY);
	if (CurrentOpacity <= 0.f)
	{
		HitEffectOverlayMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, 0.f);
		return;
	}
	
	HitEffectOverlayMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, CurrentOpacity - 0.05f);
	
	GetWorldTimerManager().SetTimerForNextTick(this, &ALLL_MonsterBase::UpdateMonsterHitVFX);
}

void ALLL_MonsterBase::DropGold(const FGameplayTag tag, int32 data)
{
	const float GoldData = DropGoldAttributeSet->GetDropGoldStat();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	for (UActorComponent* ChildComponent : Player->GetComponents())
	{
		ULLL_PlayerGoldComponent* GoldComponent = Cast<ULLL_PlayerGoldComponent>(ChildComponent);
		if(IsValid(GoldComponent))
		{
			GoldComponent->IncreaseMoney(GoldData);
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
			if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
			{
				if (DebugGameInstance->CheckPlayerAttackDebug() || DebugGameInstance->CheckPlayerSkillDebug())
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("PlayerGold %f"), GoldComponent->GetMoney()));
				}
			}
#endif
		}
	}
}
