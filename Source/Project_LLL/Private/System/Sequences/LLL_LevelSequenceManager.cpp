// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Sequences/LLL_LevelSequenceManager.h"

#include "LevelSequencePlayer.h"
#include "Constant/LLL_LevelNames.h"
#include "DataAsset/Global/LLL_GlobalLevelSequenceDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ALLL_LevelSequenceManager::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->EncounteredDelegate.AddDynamic(this, &ALLL_LevelSequenceManager::OnEncounteredCallBack);
}

void ALLL_LevelSequenceManager::OnEncounteredCallBack(ELevelSequenceType SequenceType)
{
	if (!IsValid(GetWorld()) || !IsValid(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		return;
	}

	ULevelSequence* EncounterSequence = LevelSequenceDataAsset->Sequences.Find(SequenceType)->Get();
	if (!IsValid(EncounterSequence))
	{
		return;
	}
	
	ALLL_PlayerBase* PlayerBase = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerBase->GetPlayerUIManager()->SetAllWidgetVisibility(false);
	PlayerBase->GetGoldComponent()->SetGoldWidgetVisibility(false);
	Cast<ALLL_PlayerController>(PlayerBase->GetController())->SetUIInputMode();

	SetSequence(EncounterSequence);
	GetSequencePlayer()->OnFinished.AddDynamic(this, &ALLL_LevelSequenceManager::OnFinishedCallBack);
	GetSequencePlayer()->Play();
}

void ALLL_LevelSequenceManager::OnFinishedCallBack()
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
