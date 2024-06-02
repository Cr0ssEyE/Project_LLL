// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BrainComponent.h"
#include "GameplayAbilitiesModule.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_AttributeInitializeGroupName.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GameplayTags.h"
#include "Constant/LLL_MaterialParameterName.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/LLL_ShareableNiagaraDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAIController.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/Ability/Character/Monster/Base/LLL_MGA_Charge.h"
#include "GAS/Attribute/Character/Monster/LLL_MonsterAttributeSet.h"
#include "GAS/ASC/LLL_MonsterASC.h"
#include "GAS/Attribute/Character/Player/LLL_PlayerCharacterAttributeSet.h"
#include "GAS/Attribute/DropGold/LLL_DropGoldAttributeSet.h"
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

	DropGoldAttributeSet = CreateDefaultSubobject<ULLL_DropGoldAttributeSet>(TEXT("DropGoldAttribute"));
	DropGoldEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/DropGold/BPGE_DropGold.BPGE_DropGold_C'"), EAssertionLevel::Check);

	StackedKnockBackedPower = 0.f;
	StackedKnockBackVelocity = FVector::Zero();

	MaskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask"));
	MaskMeshComponent->SetCollisionProfileName(CP_NO_COLLISION);
	MaskMeshComponent->SetupAttachment(RootComponent);

	MarkVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MarkStatusEffect"));
	MarkVFXComponent->SetupAttachment(RootComponent);
	MarkVFXComponent->SetAutoActivate(false);

	BleedingVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BleedingStatusEffect"));
	BleedingVFXComponent->SetupAttachment(RootComponent);
	BleedingVFXComponent->SetAutoActivate(false);
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

	UNiagaraSystem* MarkCountNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetShareableNiagaraDataAsset()->MarkCountNiagaraSystem;
	if (IsValid(MarkCountNiagaraSystem))
	{
		MarkVFXComponent->SetAsset(MarkCountNiagaraSystem);
		MarkVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_OVERHEAD);
	}

	UNiagaraSystem* BleedingNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetShareableNiagaraDataAsset()->BleedingNiagaraSystem;
	if (IsValid(BleedingNiagaraSystem))
	{
		BleedingVFXComponent->SetAsset(BleedingNiagaraSystem);
		BleedingVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_CHEST);
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
	IGameplayAbilitiesModule::Get().GetAbilitySystemGlobals()->GetAttributeSetInitter()->InitAttributeSetDefaults(ASC, ATTRIBUTE_INIT_MONSTER, Data, true);
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

void ALLL_MonsterBase::Charge() const
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

	if (bIsAttacking)
	{
		return;
	}

	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = Cast<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	if (!IsValid(MonsterBaseAnimInstance) || IsDOT)
	{
		return;
	}

	MonsterBaseAnimInstance->StopAllMontages(1.0f);
	PlayAnimMontage(MonsterBaseDataAsset->DamagedAnimMontage);

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
	
	DropGold(TAG_GAS_SYSTEM_DROP_GOLD, 0);

	ULLL_MonsterBaseAnimInstance* MonsterBaseAnimInstance = CastChecked<ULLL_MonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	MonsterBaseAnimInstance->StopAllMontages(1.0f);
	
	const ALLL_MonsterBaseAIController* MonsterBaseAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	MonsterBaseAIController->GetBrainComponent()->StopLogic("Monster Is Dead");

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);

	const ALLL_PlayerBase* PlayerBase = Cast<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
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

	const float DestroyTimer = MonsterAttributeSet->GetDestroyTimer();
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		Destroy();
	}), DestroyTimer, false);
}

void ALLL_MonsterBase::AddKnockBackVelocity(FVector& KnockBackVelocity, float KnockBackPower)
{
	KnockBackVelocity.Z = 0.f;
	if (KnockBackPower < 0.f)
	{
		LaunchCharacter(KnockBackVelocity, true, true);
		return;
	}
	
	if (CustomTimeDilation == 1.f)
	{
		StackedKnockBackedPower = KnockBackPower;
		if (FLLL_MathHelper::CheckFallableKnockBackPower(GetWorld(), StackedKnockBackedPower) && GetCapsuleComponent()->GetCollisionProfileName() != CP_MONSTER_FALLABLE)
		{
			GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE);
		}
		GetCharacterMovement()->Velocity = FVector::Zero();
		LaunchCharacter(KnockBackVelocity, true, true);
	}
	else
	{
		StackedKnockBackedPower += KnockBackPower;
		StackedKnockBackVelocity += KnockBackVelocity;
	}
}

void ALLL_MonsterBase::ApplyStackedKnockBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("%f"), StackedKnockBackVelocity.Length()));

	if (StackedKnockBackVelocity.Length() < 100.f)
	{
		ResetKnockBackStack();
		return;
	}
	
	if (FLLL_MathHelper::CheckFallableKnockBackPower(GetWorld(), StackedKnockBackedPower))
	{
		GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_GAS_MONSTER_FALLABLE);
	}

	// TODO: 나중에 몬스터별 최대 넉백값 같은거 나오면 수정하기
	FVector ScaledStackedKnockBackVelocity = ClampVector(StackedKnockBackVelocity, FVector::One() * -30000.f, FVector::One() * 30000.f);
	ScaledStackedKnockBackVelocity.Z = 0.f;
	GetCharacterMovement()->Velocity = FVector::Zero();
	LaunchCharacter(ScaledStackedKnockBackVelocity, true, true);

	ResetKnockBackStack();
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

void ALLL_MonsterBase::UpdateMarkVFX(uint8 NewCount, uint8 MaxCount)
{
	if (NewCount > MaxCount)
	{
		return;
	}

	UNiagaraParameterCollection* MarkParam = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetShareableNiagaraDataAsset()->MarkCountNiagaraParameterCollection;
	if (!IsValid(MarkParam))
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("표식 값 갱신: %d"), NewCount));
	MarkVFXComponent->SetFloatParameter(NS_MARK_COUNT, NewCount - 1.f);
	
	if (NewCount > 0)
	{
		MarkVFXComponent->ActivateSystem();
	}
	else
	{
		MarkVFXComponent->Deactivate();
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

void ALLL_MonsterBase::DropGold(const FGameplayTag tag, int32 data)
{
	const float GoldData = DropGoldAttributeSet->GetDropGoldStat();

	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
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
