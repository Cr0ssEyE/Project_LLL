// Fill out your copyright notice in the Description page of Project Settings.


#include "System/MapGimmick/Components/LLL_SequencerComponent.h"

#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
ULLL_SequencerComponent::ULLL_SequencerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void ULLL_SequencerComponent::PlayIntroSequence()
{
	if (!IsValid(GetWorld()) || !IntroSequencer)
	{
		return;
	}
	
	ALevelSequenceActor* IntroSequenceActor = GetWorld()->SpawnActorDeferred<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FTransform::Identity);
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;
	Settings.bHideHud = false;
	Settings.FinishCompletionStateOverride = EMovieSceneCompletionModeOverride::ForceRestoreState;
	
	if (!IsValid(IntroSequenceActor))
	{
		return;
	}

	IntroSequenceActor->PlaybackSettings = Settings;
	IntroSequenceActor->SetSequence(IntroSequencer);
	IntroSequenceActor->InitializePlayer();
	IntroSequenceActor->OnEndPlay.AddDynamic(this, &ULLL_SequencerComponent::OnSequencerEndedCallBack);
	
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->SetHidden(true);
	PlayerCharacter->GetPlayerUIManager()->SetAllWidgetVisibility(false);
	Cast<ALLL_PlayerController>(PlayerCharacter->GetController())->SetUIInputMode();
	
	IntroSequenceActor->FinishSpawning(FTransform::Identity);
}

void ULLL_SequencerComponent::OnSequencerEndedCallBack(AActor* Actor, EEndPlayReason::Type EndPlayReason)
{
	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->SetHidden(false);
	PlayerCharacter->GetPlayerUIManager()->SetAllWidgetVisibility(true);
	Cast<ALLL_PlayerController>(PlayerCharacter->GetController())->SetGameInputMode();
	
	if (bLoadToOtherLevel)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LoadLevel);
	}
}



