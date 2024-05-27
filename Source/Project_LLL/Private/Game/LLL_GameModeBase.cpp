// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LLL_GameModeBase.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_GameModeBase::ALLL_GameModeBase()
{
	DefaultPawnClass = FLLL_ConstructorHelper::FindAndGetClass<APawn>(TEXT("/Script/CoreUObject.Class'/Script/Project_LLL.LLL_PlayerBase'"));
	PlayerControllerClass = FLLL_ConstructorHelper::FindAndGetClass<APlayerController>(TEXT("/Script/CoreUObject.Class'/Script/Project_LLL.LLL_PlayerController'"));
}
