// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Lobby/LLL_LobbyCustomPoint.h"

#include "Enumeration/LLL_GameSystemEnumHelper.h"

ALLL_LobbyCustomPoint::ALLL_LobbyCustomPoint():
	CustomPointType(ELobbyCustomPointType::InteractionObject),
	CustomPointID(0)
{
	
}

void ALLL_LobbyCustomPoint::BeginPlay()
{
	Super::BeginPlay();

}


