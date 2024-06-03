// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_TutorialGimmick.h"

#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "System/MapGimmick/Components/LLL_GateSpawnPointComponent.h"
#include "System/MapGimmick/Components/LLL_PlayerSpawnPointComponent.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Constant/LLL_LevelNames.h"
#include "DataAsset/LLL_MapDataAsset.h"
#include "Entity/Character/Monster/Melee/SwordDash/LLL_SwordDash.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ALLL_TutorialGimmick::ALLL_TutorialGimmick()
{
	StageBP = FLLL_ConstructorHelper::FindAndGetClass<AActor>(PATH_TUTORIAL_MAP_BP, EAssertionLevel::Check);

	MapDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_MapDataAsset>(PATH_MAP_DATA, EAssertionLevel::Check);
}

void ALLL_TutorialGimmick::BeginPlay()
{
	Super::BeginPlay();
	PlayerTeleportNiagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MapDataAsset->TeleportParticle, FVector::ZeroVector, FRotator::ZeroRotator, MapDataAsset->ParticleScale, false, false);
	PlayerTeleportNiagara->OnSystemFinished.AddDynamic(this, &ALLL_TutorialGimmick::LoadLevel);

	StageActor = GetWorld()->SpawnActor<AActor>(StageBP, RootComponent->GetComponentLocation(), RootComponent->GetComponentRotation());
	for (USceneComponent* ChildComponent : StageActor->GetRootComponent()->GetAttachChildren())
	{
		PlayerSpawnPointComponent = Cast<ULLL_PlayerSpawnPointComponent>(ChildComponent);
		if (IsValid(PlayerSpawnPointComponent))
		{
			ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
			Player->SetActorLocationAndRotation(PlayerSpawnPointComponent->GetComponentLocation(), PlayerSpawnPointComponent->GetComponentQuat());
		}
		
		GateSpawnPointComponent = Cast<ULLL_GateSpawnPointComponent>(ChildComponent);
		if (IsValid(GateSpawnPointComponent))
		{
			Gate = GetWorld()->SpawnActor<ALLL_GateObject>(ALLL_GateObject::StaticClass(), GateSpawnPointComponent->GetComponentLocation(), GateSpawnPointComponent->GetComponentRotation());
			Gate->GateInteractionDelegate.AddUObject(this, &ALLL_TutorialGimmick::OnInteractionGate);
		}
	}
	AbilityReward = GetWorld()->SpawnActor<ALLL_AbilityRewardObject>(ALLL_AbilityRewardObject::StaticClass(), GetActorLocation(), GetActorRotation());
	AbilityReward->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::RewardDestroyed);
}

void ALLL_TutorialGimmick::OnInteractionGate(const FRewardDataTable* Data)
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->DisableInput(GetWorld()->GetFirstPlayerController());
	PlayerTeleportNiagara->SetWorldLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation());
	PlayerTeleportNiagara->ActivateSystem();
}

void ALLL_TutorialGimmick::RewardDestroyed(AActor* DestroyedActor)
{
	ALLL_SwordDash* Monster = GetWorld()->SpawnActor<ALLL_SwordDash>(ALLL_SwordDash::StaticClass(), FVector(200.0f, 200.0f, 200.0f), GetActorRotation());
	Monster->OnDestroyed.AddDynamic(this, &ALLL_TutorialGimmick::MonsterDestroyed);
}

void ALLL_TutorialGimmick::MonsterDestroyed(AActor* DestroyedActor)
{
	Gate->SetActivate();
}

void ALLL_TutorialGimmick::LoadLevel(UNiagaraComponent* InNiagaraComponent)
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorHiddenInGame(true);
	UGameplayStatics::OpenLevel(this, LEVEL_TEST);
}

