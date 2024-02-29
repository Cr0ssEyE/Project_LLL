// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ProtoGameModeBase.h"
#include "Util/LLLConstructorHelper.h"

ALLL_ProtoGameModeBase::ALLL_ProtoGameModeBase()
{
	DefaultPawnClass = FLLLConstructorHelper::FindAndGetClass<APawn>(TEXT("/Script/CoreUObject.Class'/Script/Project_LLL.LLL_PlayerBase'"));
	PlayerControllerClass = FLLLConstructorHelper::FindAndGetClass<APlayerController>(TEXT("/Script/CoreUObject.Class'/Script/Project_LLL.LLL_PlayerController'"));
}
