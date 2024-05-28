// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/LLL_TutorialGimmick.h"

#include "Constant/LLL_FilePath.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Object/Interactive/LLL_AbilityRewardObject.h"
#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"
#include "System/MapGimmick/LLL_GateSpawnPointComponent.h"
#include "System/MapGimmick/LLL_PlayerSpawnPointComponent.h"
#include "Util/LLL_ConstructorHelper.h"
#include <Kismet/GameplayStatics.h>
#include <Constant/LLL_LevelNames.h>

ALLL_TutorialGimmick::ALLL_TutorialGimmick()
{
	StageBP = FLLL_ConstructorHelper::FindAndGetClass<AActor>(PATH_TUTORIAL_MAP_BP, EAssertionLevel::Check);
}

void ALLL_TutorialGimmick::BeginPlay()
{
	Super::BeginPlay();
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

void ALLL_TutorialGimmick::OnInteractionGate(FRewardDataTable* Data)
{
	LoadLevel();
}

void ALLL_TutorialGimmick::RewardDestroyed(AActor* DestroyedActor)
{
	Gate->SetActivate();
}

void ALLL_TutorialGimmick::LoadLevel()
{
	UGameplayStatics::OpenLevel(this, LEVEL_TEST);
}

