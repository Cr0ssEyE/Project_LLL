// Fill out your copyright notice in the Description page of Project Settings.


#include "System/LLL_EncounterLevelSequenceActor.h"

#include "LevelSequencePlayer.h"
#include "Constant/LLL_LevelNames.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ALLL_EncounterLevelSequenceActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->EncountedDelegate.AddDynamic(this, &ALLL_EncounterLevelSequenceActor::OnEncountedCallBack);
}

void ALLL_EncounterLevelSequenceActor::OnEncountedCallBack()
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}

	ALLL_PlayerBase* PlayerBase = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerBase->GetPlayerUIManager()->SetAllWidgetVisibility(false);
	PlayerBase->GetGoldComponent()->SetGoldWidgetVisibility(false);
	Cast<ALLL_PlayerController>(PlayerBase->GetController())->SetUIInputMode();
	
	GetSequencePlayer()->OnFinished.AddDynamic(this, &ALLL_EncounterLevelSequenceActor::OnFinishedCallBack);
	GetSequencePlayer()->Play();
}

void ALLL_EncounterLevelSequenceActor::OnFinishedCallBack()
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}
	
	// ALLL_PlayerBase* PlayerBase = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	// PlayerBase->GetPlayerUIManager()->SetAllWidgetVisibility(true);
	// PlayerBase->GetGoldComponent()->SetGoldWidgetVisibility(true);
	// Cast<ALLL_PlayerController>(PlayerBase->GetController())->SetGameInputMode();
	
	UGameplayStatics::OpenLevel(GetWorld(), LEVEL_CREDIT);
}
