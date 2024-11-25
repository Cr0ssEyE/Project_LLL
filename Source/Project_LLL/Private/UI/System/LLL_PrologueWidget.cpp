// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/System/LLL_PrologueWidget.h"

#include "Constant/LLL_LevelNames.h"
#include "Kismet/GameplayStatics.h"

void ULLL_PrologueWidget::OpenStage1Level()
{
	UGameplayStatics::OpenLevel(this, LEVEL_STAGE1_02);
}
