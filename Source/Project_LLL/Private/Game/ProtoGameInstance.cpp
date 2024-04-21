// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ProtoGameInstance.h"

#include "Constant/LLL_FilePath.h"
#include "Util/LLLConstructorHelper.h"

UProtoGameInstance::UProtoGameInstance()
{
	AbilityDataTable = FLLLConstructorHelper::FindAndGetObject<UDataTable>(PATH_ABILITY_DATA_TABLE, EAssertionLevel::Check);

	RewardDataTable = FLLLConstructorHelper::FindAndGetObject<UDataTable>(PATH_REWARD_DATA_TABLE, EAssertionLevel::Check);
}
