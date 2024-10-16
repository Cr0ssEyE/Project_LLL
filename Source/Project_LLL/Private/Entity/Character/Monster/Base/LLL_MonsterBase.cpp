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
#include "Entity/Character/Monster/DPSTester/LLL_DPSTester.h"
#include "Entity/Character/Monster/Melee/BombSkull/LLL_BombSkull.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Thrown/Base/LLL_ThrownObject.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/ASC/LLL_MonsterASC.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
#include "Navigation/PathFollowingComponent.h"
#include "System/MapGimmick/LLL_FallableWallGimmick.h"
#include "UI/Entity/Character/Base/LLL_CharacterStatusWidget.h"
#include "UI/Entity/Character/Monster/LLL_FloatingDamageActor.h"
#include "Util/LLL_AbilityDataHelper.h"
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

	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	DropGoldEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'"), EAssertionLevel::Check);

	StackVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("StackStatusEffect"));
	StackVFXComponent->SetupAttachment(RootComponent);
	StackVFXComponent->SetAutoActivate(false);

	BleedingVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BleedingStatusEffect"));
	BleedingVFXComponent->SetupAttachment(RootComponent);
	BleedingVFXComponent->SetAutoActivate(false);

	AttributeInitId = ATTRIBUTE_INIT_MONSTER;
	MaxBleedingStack = 5;

	FloatingDamageActor = FLLL_ConstructorHelper::FindAndGetClass<AActor>(PATH_UI_FLOATING_DAMAGE_ACTOR, EAssertionLevel::Check);
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	EffectContextHandle.AddInstigator(this, this);
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

	if (GetActorLocation().Z < -1000.0f)
	{
		if (Cast<ALLL_DPSTester>(this))
		{
			Destroy();
		}
		else if (!CheckCharacterIsDead())
		{
			Dead();
		}
	}

	if (bIsKnockBacking)
	{
		FVector VelocityWithKnockBack = GetVelocity();
		
		// 넉백 끝났을때 처리
		ALLL_MonsterBaseAIController* MonsterAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
		
		bool IsMoving = false;
		if (IsValid(MonsterAIController))
		{
			IsMoving = CastChecked<ALLL_MonsterBaseAIController>(GetController())->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Moving;
		}
		
		if ((VelocityWithKnockBack == FVector::ZeroVector || IsMoving) && bStartKnockBackVelocity)
		{
			CustomTimeDilation = 1.f;
			UE_LOG(LogTemp, Log, TEXT("%s가 넉백 끝"), *GetName())
			bIsKnockBacking = false;
			bStartKnockBackVelocity = false;
			DeflectCount = 0;
			KnockBackSender = nullptr;
			bKnockBackTargetDamaged = false;
			bKnockBackCauserDamaged = false;
			bBleedingTransmissionTargetDamaged = false;

			if (IsValid(MonsterAIController))
			{
				if (!CheckCharacterIsDead())
				{
					MonsterAIController->StartLogic();
					MonsterAIController->ResumeMove(FAIRequestID::AnyRequest);
				}
			}
		}
		else
		{
			bStartKnockBackVelocity = true;
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

	const int32 Data = Id * 100 + AbilityLevel;
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

		ALLL_ManOfStrength* ManOfStrength = Cast<ALLL_ManOfStrength>(GetOwner());
		ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Other);
		if (IsValid(ManOfStrength) && IsValid(Player))
		{
			const ULLL_ManOfStrengthDataAsset* ManOfStrengthDataAsset = CastChecked<ULLL_ManOfStrengthDataAsset>(ManOfStrength->GetCharacterDataAsset());
			const ULLL_MonsterAttributeSet* ManOfStrengthAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrength->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
			const float OffencePower = ManOfStrengthAttributeSet->GetManOfStrengthThrowOffencePower();
			
			FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
			EffectContextHandle.AddSourceObject(this);
			EffectContextHandle.AddInstigator(ManOfStrength, this);
			const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ManOfStrengthDataAsset->ThrowDamageEffect, ManOfStrength->GetAbilityLevel(), EffectContextHandle);
			if (EffectSpecHandle.IsValid())
			{
				EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
				UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *Other->GetName())
				ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			}
		}
		else
		{
			ManOfStrength = Cast<ALLL_ManOfStrength>(Other);
			Player = Cast<ALLL_PlayerBase>(GetOwner());
			if (IsValid(ManOfStrength) && IsValid(Player))
			{
				const ULLL_ManOfStrengthDataAsset* ManOfStrengthDataAsset = CastChecked<ULLL_ManOfStrengthDataAsset>(ManOfStrength->GetCharacterDataAsset());
				const ULLL_MonsterAttributeSet* OwnerMonsterAttributeSet = CastChecked<ULLL_MonsterAttributeSet>(ManOfStrength->GetAbilitySystemComponent()->GetAttributeSet(ULLL_MonsterAttributeSet::StaticClass()));
				const float OffencePower = OwnerMonsterAttributeSet->GetManOfStrengthThrowOffencePower();
			
				FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
				EffectContextHandle.AddSourceObject(this);
				EffectContextHandle.AddInstigator(Player, this);
				const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(ManOfStrengthDataAsset->ThrowDamageEffect, ManOfStrength->GetAbilityLevel(), EffectContextHandle);
				if (EffectSpecHandle.IsValid())
				{
					EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
					UE_LOG(LogTemp, Log, TEXT("%s에게 데미지"), *Other->GetName())
					ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, ManOfStrength->GetAbilitySystemComponent());
				}
				
				ManOfStrength->Stun();
			}
		}
		
		SetOwner(nullptr);
		Dead();
	}
	else if (bIsKnockBacking)
	{
		if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
			if (!Cast<ALLL_BaseCharacter>(Other) && !Cast<ALLL_ThrownObject>(Other) && !Cast<ALLL_AbilityObject>(Other) && !Cast<ALLL_FallableWallGimmick>(Other))
			{
				float DotProduct = FVector::DotProduct(HitNormal, FVector::UpVector);
				float AngleInRadians = FMath::Acos(DotProduct);
				float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);
				if (AngleInDegrees > 45.0f)
				{
					UE_LOG(LogTemp, Log, TEXT("%s가 %s에 부딪혀 넉백 피해입음"), *GetName(), *Other->GetName())
					DamageKnockBackCauser(Player);

					// 리바운드 이누리아
					if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_DEFLECT_BY_WALL) && DeflectCount < Player->GetDeflectCount())
					{
						// 반사 벡터 구하기
						LastKnockBackVelocity = LastKnockBackVelocity - 2 * FVector::DotProduct(LastKnockBackVelocity, HitNormal) * HitNormal;
						DeflectCount++;
						UE_LOG(LogTemp, Log, TEXT("벽에 %d번 튕김"), DeflectCount)
						AddKnockBackVelocity(LastKnockBackVelocity, LastKnockBackPower);
						bStartKnockBackVelocity = false;
						bKnockBackCauserDamaged = false;
					}
				}
			}

			ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(Other);
			if (IsValid(OtherMonster))
			{
				UE_LOG(LogTemp, Log, TEXT("%s와 %s가 부딪혀 서로 넉백 피해입음"), *GetName(), *Other->GetName())
				DamageKnockBackTarget(Player, OtherMonster);
				DamageKnockBackCauser(Player);

				if ((!IsValid(KnockBackSender) || OtherMonster != KnockBackSender) && !OtherMonster->IsKnockBacking())
				{
					const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
				
					// 연쇄 작용 이누리아
					if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_KNOCK_BACK_TRANSMISSION))
					{
						float OffencePower = Player->GetKnockBackTransmissionOffencePower();
						OffencePower = FLLL_AbilityDataHelper::CalculateOffencePower(OffencePower, Player);

						FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
						EffectContextHandle.AddSourceObject(this);
						EffectContextHandle.AddInstigator(Player, this);
						const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->KnockBackTransmissionDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
						if (EffectSpecHandle.IsValid())
						{
							EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, OffencePower);
							UE_LOG(LogTemp, Log, TEXT("연쇄 작용으로 %s에게 %f만큼 데미지"), *Other->GetName(), OffencePower)
							ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, OtherMonster->GetAbilitySystemComponent());
						}

						FVector Direction = (OtherMonster->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
					
						float KnockBackPower = Player->GetKnockBackTransmissionKnockBackPower();
						KnockBackPower = FLLL_AbilityDataHelper::CalculateKnockBackPower(KnockBackPower, Player);
			
						UE_LOG(LogTemp, Log, TEXT("연쇄 작용으로 %s에게 %f만큼 넉백"), *Other->GetName(), KnockBackPower)
						FVector LaunchVelocity = FLLL_MathHelper::CalculateLaunchVelocity(Direction, KnockBackPower);
						OtherMonster->AddKnockBackVelocity(LaunchVelocity, KnockBackPower);
					}

					// 피의 역병 이누리아
					if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_BLEEDING_TRANSMISSION) && ASC->HasMatchingGameplayTag(TAG_GAS_STATUS_BLEEDING) && !bBleedingTransmissionTargetDamaged)
					{
						bBleedingTransmissionTargetDamaged = true;
						OtherMonster->SetBleedingStack(OtherMonster->GetBleedingStack() + Player->GetBleedingTransmissionStack() - 1);
					
						FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
						EffectContextHandle.AddSourceObject(this);
						EffectContextHandle.AddInstigator(Player, this);
						const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->BleedingTransmissionDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
						if (EffectSpecHandle.IsValid())
						{
							EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, BleedingTransmissionOffencePower);
							FLLL_AbilityDataHelper::SetBleedingPeriodValue(Player, CastChecked<ULLL_ExtendedGameplayEffect>(PlayerDataAsset->BleedingTransmissionDamageEffect.GetDefaultObject()));
							if (!FLLL_AbilityDataHelper::CheckBleedingExplosion(Player, OtherMonster, this))
							{
								ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, OtherMonster->GetAbilitySystemComponent());
							}
						}
					}
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

void ALLL_MonsterBase::Damaged(AActor* Attacker, bool IsDOT, float Damage)
{
	Super::Damaged(Attacker, IsDOT, Damage);

	ShowDamageValue(Damage);

	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(Attacker))
	{
		FGameplayEventData PayloadData;
		PayloadData.Instigator = Attacker;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_ANY_ATTACK_SUCCESS, PayloadData);
	}
	
	if (Cast<ALLL_DPSTester>(this))
	{
		return;
	}
	
	ShowHitEffect();
	RecognizePlayerToAroundMonster();
	
	if (Cast<ALLL_BossMonster>(this))
	{
		return;
	}

	if (bIsAttacking || IsDOT)
	{
		return;
	}

	Stun();
}

void ALLL_MonsterBase::Dead()
{
	Super::Dead();
	
	CharacterAnimInstance->StopAllMontages(1.0f);
	//GetMesh()->SetCustomDepthStencilValue(0);
	
	DropGold(TAG_GAS_SYSTEM_DROP_GOLD, 0);
	
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	MonsterBaseAIController->StopLogic("Monster Is Dead");

	if (!IsValid(MonsterBaseDataAsset->DeadAnimMontage))
	{
		GetCapsuleComponent()->SetCollisionProfileName(CP_RAGDOLL);
		GetMesh()->SetCollisionProfileName(CP_RAGDOLL);
		
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
	}
	else
	{
		PlayAnimMontage(MonsterBaseDataAsset->DeadAnimMontage);
	}

	MonsterStatusWidgetComponent->SetHiddenInGame(true);

	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
		TempNiagaraComponent->Deactivate();
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
	ALLL_MonsterBaseAIController* MonsterAIController = Cast<ALLL_MonsterBaseAIController>(GetController());
	if (IsValid(MonsterAIController))
	{
		MonsterAIController->PauseMove(FAIRequestID::AnyRequest);
	}
	
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
		if (IsValid(MonsterAIController))
		{
			MonsterAIController->StopLogic("Monster Is Fallable");
		}
		return;
	}

	if (CheckCharacterIsDead() && !Cast<ALLL_BombSkull>(this))
	{
		return;
	}
	
	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		const UAbilitySystemComponent* PlayerASC = Player->GetAbilitySystemComponent();
		
		// 넘치는 힘 이누리아
		if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_FASTER_KNOCK_BACK))
		{
			CustomTimeDilation = 1 + Player->GetFasterKnockBackSpeedRateIncrease();
		}
		
		FGameplayEventData PayloadData;
		TArray<TWeakObjectPtr<AActor>> NewActors;
		NewActors.Emplace(this);
		FGameplayAbilityTargetDataHandle NewTargetDataHandle;
		FGameplayAbilityTargetData_ActorArray* NewTargetData = new FGameplayAbilityTargetData_ActorArray();
		NewTargetData->TargetActorArray = NewActors;
		NewTargetDataHandle.Add(NewTargetData);
		PayloadData.TargetData = NewTargetDataHandle;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, TAG_GAS_MONSTER_KNOCK_BACKED, PayloadData);
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

void ALLL_MonsterBase::Attack()
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
				if (ALLL_MonsterBaseAIController* MonsterAIController = Cast<ALLL_MonsterBaseAIController>(Monster->GetController()))
				{
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

	HitEffectOverlayMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, 0.1f);
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
	if (!bKnockBackTargetDamaged)
	{
		UE_LOG(LogTemp, Log, TEXT("%s에 부딪혀 넉백 피해입음"), *Monster->GetName())
		
		bKnockBackTargetDamaged = true;
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
	if (!bKnockBackCauserDamaged)
	{
		bKnockBackCauserDamaged = true;
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

void ALLL_MonsterBase::Stun()
{
	ULLL_BaseCharacterAnimInstance* AnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
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
	}

	const TArray<UNiagaraComponent*> TempNiagaraComponents = NiagaraComponents;
	for (auto TempNiagaraComponent : TempNiagaraComponents)
	{
		if (!IsValid(TempNiagaraComponent))
		{
			continue;
		}
		
		TempNiagaraComponent->Deactivate();
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

void ALLL_MonsterBase::ShowDamageValue(const float Damage) const
{
	ALLL_FloatingDamageActor* FloatingDamage = GetWorld()->SpawnActor<ALLL_FloatingDamageActor>(FloatingDamageActor);
	FloatingDamage->SetActorLocation(GetActorLocation() + FVector(0, 0, 100.0f));
	FloatingDamage->SetWidgetText(Damage);
}

void ALLL_MonsterBase::ToggleBleedingTrigger()
{
	bBleedingTrigger = !bBleedingTrigger;
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
	
	HitEffectOverlayMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, CurrentOpacity - 0.005f);
	
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
