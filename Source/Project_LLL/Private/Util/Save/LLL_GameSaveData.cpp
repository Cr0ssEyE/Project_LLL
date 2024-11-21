// Fill out your copyright notice in the Description page of Project Settings.


#include "Constant/LLL_GameplayInfo.h"
#include "Constant/LLL_LevelNames.h"
#include "Util/Save/LLL_SaveGameData.h"

ULLL_SaveGameData::ULLL_SaveGameData() :
SaveFileName(DEFAULT_FILE_NAME),
SaveFileIndex(DEFAULT_FILE_INDEX),
StoryProgress(STORY_BEGIN),
LastPlayedLevelName(LEVEL_PROLOGUE)
{
	
}
