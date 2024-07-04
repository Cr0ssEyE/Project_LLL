// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_TutorialGimmick.h"

#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"

#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "System/MapGimmick/Components/LLL_GateSpawnPointComponent.h"
#include "System/MapGimmick/Components/LLL_PlayerSpawnPointComponent.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Constant/LLL_LevelNames.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DataAsset/LLL_TutorialMapDataAsset.h"
#include "Game/LLL_GameInstance.h"
#include "UI/System/LLL_TutorialWidget.h"

ALLL_TutorialGimmick::ALLL_TutorialGimmick()
{
	TutorialDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_TutorialMapDataAsset>(PATH_TUTORIAL_MAP_DATA, EAssertionLevel::Check);
	bIsActiveSkill = false; 
	bIsActiveDash = false;
	ChargeSkillGaugeEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(PATH_SKILL_GAUGE_EFFECT, EAssertionLevel::Check);
}

void ALLL_TutorialGimmick::BeginPlay()
{
	Super::BeginPlay();
	PlayerTeleportNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TutorialDataAsset->TeleportParticle, FVector::ZeroVector, FRotator::ZeroRotator, TutorialDataAsset->ParticleScale, false, false);
	PlayerTeleportNiagara->OnSystemFinished.AddDynamic(this, &ALLL_TutorialGimmick::LoadLevel);

	AActor* StageActor = GetWorld()->SpawnActor<AActor>(TutorialDataAsset->BPMap, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	StageActors.Emplace(StageActor);
	for (USceneComponent* ChildComponent : StageActor->GetRootComponent()->GetAttachChildren())
	{
		PlayerSpawnPointComponent = Cast<ULLL_PlayerSpawnPointComponent>(ChildComponent);
		if (IsValid(PlayerSpawnPointComponent))
		{
			ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
			Player->SetActorLocationAndRotation(PlayerSpawnPointComponent->GetComponentLocation(), PlayerSpawnPointComponent->GetComponentQuat());
		}
	}

	FTransform StageActorTransform;
	StageActorTransform.SetLocation(TutorialDataAsset->AttackStageLocation);
	StageActorTransform.SetRotation(RootComponent->GetComponentRotation().Quaternion());
	
	StageActor = GetWorld()->SpawnActorDeferred<AActor>(TutorialDataAsset->BPMap, StageActorTransform);
	StageActor->OnActorBeginOverlap.AddDynamic(this, &ALLL_TutorialGimmick::BeginOverlapAttackTutorial);
	StageActor->FinishSpawning(StageActorTransform);
	StageActors.Emplace(StageActor);
	
	TutorialWidgetClass = TutorialDataAsset->TutorialWidgetClass;

	if(IsValid(TutorialWidgetClass))
	{
		TutorialWidget = CastChecked<ULLL_TutorialWidget>(CreateWidget(GetWorld(), TutorialWidgetClass));
		TutorialWidget->AddToViewport();
		TutorialWidget->SetDashTutorial();
	}
}

void ALLL_TutorialGimmick::BeginOverlapAttackTutorial(AActor* OverlappedActor, AActor* OtherActor)
{
	if (bIsActiveDash)
	{
		return;
	}

	if (OtherActor != GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		return;
	}
	
	bIsActiveDash = true;
	FVector Vector = TutorialDataAsset->AttackStageLocation;
	Vector.Z += 150;
	ALLL_SwordDash* Monster = GetWorld()->SpawnActor<ALLL_SwordDash>(ALLL_SwordDash::StaticClass(), Vector, GetActorRotation());
	Monster->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::FinalMapSpawn);
	TutorialWidget->SetFightTutorial();
}

void ALLL_TutorialGimmick::FinalMapSpawn(AActor* DestroyedActor)
{
	TutorialWidget->SetChaseTutorial();
	AActor* StageActor = GetWorld()->SpawnActor<AActor>(TutorialDataAsset->BPMap, TutorialDataAsset->FinalStageLocation, RootComponent->GetComponentRotation());
	StageActors.Emplace(StageActor);
	for (USceneComponent* ChildComponent : StageActor->GetRootComponent()->GetAttachChildren())
	{
		GateSpawnPointComponent = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (IsValid(GateSpawnPointComponent))
		{
			Gate = GetWorld()->SpawnActor<ALLL_GateObject>(ALLL_GateObject::StaticClass(), GateSpawnPointComponent->GetComponentLocation(), GateSpawnPointComponent->GetComponentRotation());
			Gate->FadeOutDelegate.AddUObject(this, &ALLL_TutorialGimmick::OnInteractionGate);
		}
	}
	FVector Vector = TutorialDataAsset->FinalStageLocation;
	Vector.Z += 150;
	ALLL_SwordDash* Monster = GetWorld()->SpawnActor<ALLL_SwordDash>(ALLL_SwordDash::StaticClass(), Vector, GetActorRotation());
	Monster->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::FinalMonsterSpawn);
}

void ALLL_TutorialGimmick::FinalMonsterSpawn(AActor* DestroyedActor)
{
	TutorialWidget->SetSkillTutorial();

	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FGameplayEffectContextHandle EffectContextHandle = Player->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Player);
	const FGameplayEffectSpecHandle EffectSpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(ChargeSkillGaugeEffect, 1.0, EffectContextHandle);
	Player->GetAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);

	FVector Vector = TutorialDataAsset->FinalStageLocation;
	
	Vector.Z += 150;
	while (Monsters.Num() < 3)
	{
		float RandomSpawnPointX = FMath::RandRange(-300.f, 300.f);
		float RandomSpawnPointY = FMath::RandRange(-300.f, 300.f);
		ALLL_SwordDash* Monster = GetWorld()->SpawnActor<ALLL_SwordDash>(ALLL_SwordDash::StaticClass(), Vector + FVector(RandomSpawnPointX, RandomSpawnPointY, 0.f), GetActorRotation());
		if (!IsValid(Monster))
		{
			continue;
		}
		
		Monster->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::MonsterDestroyed);
		Monsters.Emplace(Monster);
	}

	FTimerHandle SetTutorialTimerHandle;
	GetWorldTimerManager().SetTimer(SetTutorialTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&] {
		TutorialWidget->SetSkillChargeTutorial();
	}), 3.0f, false);
}

void ALLL_TutorialGimmick::MonsterDestroyed(AActor* DestroyedActor)
{
	Monsters.Remove(DestroyedActor);
	if (Monsters.Num() > 0)
	{
		return;
	}
	const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
	FVector Vector = TutorialDataAsset->FinalStageLocation;
	Vector.Z += 150;
	AbilityReward = GetWorld()->SpawnActor<ALLL_RewardObject>(ALLL_RewardObject::StaticClass(), Vector, GetActorRotation());
	AbilityReward->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::RewardDestroyed);
	AbilityReward->SetInformation(GameInstance->GetRewardDataTable()[0]);
	TutorialWidget->SetRewardTutorial();
}

void ALLL_TutorialGimmick::RewardDestroyed(AActor* DestroyedActor)
{
	Gate->SetActivate();
}

void ALLL_TutorialGimmick::OnInteractionGate()
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->DisableInput(GetWorld()->GetFirstPlayerController());
	PlayerTeleportNiagara->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	PlayerTeleportNiagara->ActivateSystem();
}

void ALLL_TutorialGimmick::LoadLevel(UNiagaraComponent* InNiagaraComponent)
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorHiddenInGame(true);
	UGameplayStatics::OpenLevel(this, LEVEL_TEST);
}

