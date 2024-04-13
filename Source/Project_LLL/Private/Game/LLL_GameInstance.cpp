// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameInstance.h"

#include "Interface/LLL_PlayerDependencyInterface.h"

ULLL_GameInstance::ULLL_GameInstance()
{
	
}

void ULLL_GameInstance::AssignPlayerDependencyActors(AActor* Actor)
{
	ILLL_PlayerDependencyActorInterface* PlayerDependencyActor = Cast<ILLL_PlayerDependencyActorInterface>(Actor);
	if (PlayerDependencyActor)
	{
		PlayerDependencyActors.Emplace(Actor);
		PlayerActorAssignedDelegate.Broadcast(Actor);
	}
}
