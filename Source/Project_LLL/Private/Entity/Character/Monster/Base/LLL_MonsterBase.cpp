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
#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"
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

	BleedingVFXComponent1 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BleedingStatusEffect1"));
	BleedingVFXComponent1->SetupAttachment(RootComponent);
	BleedingVFXComponent1->SetAutoActivate(false);
	
	BleedingVFXComponent2 = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BleedingStatusEffect2"));
	BleedingVFXComponent2->SetupAttachment(RootComponent);
	BleedingVFXComponent2->SetAutoActivate(false);

	AttributeInitId = ATTRIBUTE_INIT_MONSTER;
	MaxBleedingStack = 5;

	FloatingDamageActor = FLLL_ConstructorHelper::FindAndGetClass<AActor>(PATH_UI_FLOATING_DAMAGE_ACTOR, EAssertionLevel::Check);
}

void ALLL_MonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	MonsterBaseDataAsset = Cast<ULLL_MonsterBaseDataAsset>(CharacterDataAsset);
	GetMesh()->SetReceivesDecals(false);

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

	UNiagaraSystem* BleedingNiagaraSystem1 = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->BleedingNiagaraSystem1;
	if (IsValid(BleedingNiagaraSystem1))
	{
		BleedingVFXComponent1->SetAsset(BleedingNiagaraSystem1);
		BleedingVFXComponent1->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_CHEST);
	}
	
	UNiagaraSystem* BleedingNiagaraSystem2 = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->BleedingNiagaraSystem2;
	if (IsValid(BleedingNiagaraSystem2))
	{
		BleedingVFXComponent2->SetAsset(BleedingNiagaraSystem2);
		BleedingVFXComponent2->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_CHEST);
	}
	
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
		
		if ((VelocityWithKnockBack == FVector::ZeroVector || IsMoving) && bStartKnockBackVelocity && (Cast<ALLL_DPSTester>(this) || !CharacterAnimInstance->Montage_IsPlaying(MonsterBaseDataAsset->DamagedAnimMontage)))
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
	
	int32 Data = Id * 100 + AbilityLevel;
	if (bIsElite)
	{
		Data += 100;
		SetOutline();
	}
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(ASC, AttributeInitId, Data, true);
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
					DamageKnockBackCauser(Player, Other);

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
				DamageKnockBackCauser(Player, Other);

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
					if (PlayerASC->HasMatchingGameplayTag(TAG_GAS_HAVE_BLEEDING_TRANSMISSION) && !bBleedingTransmissionTargetDamaged)
					{
						bBleedingTransmissionTargetDamaged = true;
						OtherMonster->SetBleedingStack(OtherMonster->GetBleedingStack() + Player->GetBleedingTransmissionStack() - 1);
						const ULLL_AbnormalStatusAttributeSet* AbnormalStatusAttributeSet = Cast<ULLL_AbnormalStatusAttributeSet>(PlayerASC->GetAttributeSet(ULLL_AbnormalStatusAttributeSet::StaticClass()));
					
						FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
						EffectContextHandle.AddSourceObject(this);
						EffectContextHandle.AddInstigator(Player, this);
						const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->BleedingTransmissionDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
						if (EffectSpecHandle.IsValid())
						{
							EffectSpecHandle.Data->SetSetByCallerMagnitude(TAG_GAS_ABILITY_VALUE_OFFENCE_POWER, AbnormalStatusAttributeSet->GetBleedingStatusDamage());
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

	if (bIsAttacking || IsDOT)
	{
		return;
	}

	if (Cast<ALLL_BossMonster>(this) || bIsElite)
	{
		return;
	}

	Stun();
}

void ALLL_MonsterBase::Dead()
{
	Super::Dead();
	
	if (!Cast<ALLL_BombSkull>(this))
	{
		GetMesh()->SetCollisionResponseToChannel(ECC_ENEMY_HIT, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_ENEMY_HIT, ECR_Ignore);
	}
	
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
		TempNiagaraComponent->SetVisibility(false);
		NiagaraComponents.Remove(TempNiagaraComponent);
	}
	BleedingVFXComponent1->SetHiddenInGame(true);
	BleedingVFXComponent2->SetHiddenInGame(true);
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

		FTimerHandle DeadHandle;
		GetWorldTimerManager().SetTimer(DeadHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
			Dead();
			Destroy();
		}), 1.0f, false);
		return;
	}

	if ((CheckCharacterIsDead() && !Cast<ALLL_BombSkull>(this)) || bIsElite || Cast<ALLL_BossMonster>(this))
	{
		return;
	}

	if (IsValid(MonsterAIController))
	{
		MonsterAIController->PauseMove(FAIRequestID::AnyRequest);
	}
	
	if (ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
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
	if (!IsValid(HitEffectMaterialInstance))
	{
		HitEffectMaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
		GetMesh()->SetMaterial(0, HitEffectMaterialInstance);
		for (auto ChildComponent : GetMesh()->GetAttachChildren())
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ChildComponent))
			{
				StaticMeshComponent->SetMaterial(0, HitEffectMaterialInstance);
			}
		}
	}

	HitEffectMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, 0.9f);
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

void ALLL_MonsterBase::DamageKnockBackTarget(ALLL_PlayerBase* Player, const ALLL_MonsterBase* OtherMonster)
{
	if (!bKnockBackTargetDamaged)
	{
		UE_LOG(LogTemp, Log, TEXT("%s에 부딪혀 넉백 피해입음"), *OtherMonster->GetName())
		
		bKnockBackTargetDamaged = true;
		const ULLL_PlayerBaseDataAsset* PlayerDataAsset = CastChecked<ULLL_PlayerBaseDataAsset>(Player->GetCharacterDataAsset());
		
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		EffectContextHandle.AddInstigator(Player, this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(PlayerDataAsset->KnockBackTargetDamageEffect, Player->GetAbilityLevel(), EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, OtherMonster->GetAbilitySystemComponent());
		}
	}
}

void ALLL_MonsterBase::DamageKnockBackCauser(ALLL_PlayerBase* Player, AActor* Other)
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

		if (ALLL_BreakableObjectBase* BreakableObject = Cast<ALLL_BreakableObjectBase>(Other))
		{
			BreakableObject->ReceivePlayerAttackOrKnockBackedMonster();
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
		TempNiagaraComponent->SetVisibility(false);
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

void ALLL_MonsterBase::SetOutline()
{
	UMaterialInstanceDynamic* OverlayMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetOverlayMaterial(), this);
	if(IsValid(OverlayMaterial))
	{
		GetMesh()->SetOverlayMaterial(OverlayMaterial);
		OverlayMaterial->SetScalarParameterValue(TEXT("Thickness"), 0.2f);
	}
}

bool ALLL_MonsterBase::CheckBleedingTrigger()
{
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (BleedingTriggerCount == Player->GetBleedingTriggerMaxCount() - 1)
	{
		return true;
	}
	return false;
}

void ALLL_MonsterBase::IncreaseBleedingTrigger()
{
	const ALLL_PlayerBase* Player = Cast<ALLL_PlayerBase>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (BleedingTriggerCount < Player->GetBleedingTriggerMaxCount() - 1)
	{
		BleedingTriggerCount++;
	}
}

void ALLL_MonsterBase::ResetBleedingTrigger()
{
	BleedingTriggerCount = 0;
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

	UE_LOG(LogTemp, Log, TEXT("표식 값 갱신: %d"), NewCount);

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
		if (BleedingStack < MaxBleedingStack)
		{
			BleedingVFXComponent1->ActivateSystem();
			BleedingVFXComponent1->SetVisibility(true);
		}
		else
		{
			BleedingVFXComponent2->ActivateSystem();
			BleedingVFXComponent2->SetVisibility(true);
		}
			
	}
	else
	{
		if (GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(FGameplayTagContainer(TAG_GAS_STATUS_BLEEDING)))
		{
			return;
		}
		BleedingVFXComponent1->Deactivate();
		BleedingVFXComponent2->Deactivate();
		BleedingVFXComponent1->SetVisibility(false);
		BleedingVFXComponent2->SetVisibility(false);
	}
}

void ALLL_MonsterBase::UpdateMonsterHitVFX()
{
	float CurrentOpacity = HitEffectMaterialInstance->K2_GetScalarParameterValue(MAT_PARAM_OPACITY);
	if (CurrentOpacity <= 0.f)
	{
		HitEffectMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, 0.f);
		return;
	}
	
	HitEffectMaterialInstance->SetScalarParameterValue(MAT_PARAM_OPACITY, CurrentOpacity - 0.005f);
	
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
