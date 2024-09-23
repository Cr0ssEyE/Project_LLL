// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Lobby/LLL_WorldTreeObject.h"

#include "Constant/LLL_GeneralConstants.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Entity/Character/Player/LLL_PlayerController.h"
#include "UI/System/LLL_PermanentEnhancementWidget.h"

ALLL_WorldTreeObject::ALLL_WorldTreeObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ALLL_WorldTreeObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALLL_WorldTreeObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);

	ALLL_PlayerBase* PlayerCharacter = CastChecked<ALLL_PlayerBase>(InteractedActor);

	if(!IsValid(PermanentEnhancementWidgetClass))
	{
		return;
	}
	
	ULLL_PermanentEnhancementWidget* PermanentEnhancementWidget = CastChecked<ULLL_PermanentEnhancementWidget>(CreateWidget(GetWorld(), PermanentEnhancementWidgetClass));
	PermanentEnhancementWidget->SetVisibility(ESlateVisibility::Visible);
	PermanentEnhancementWidget->SetIsEnabled(true);

	// 영구강화 관련 UI 나오면 데이터 연동하기
	SetUpWidgetInfo(*PermanentEnhancementWidget);
	
	PermanentEnhancementWidget->AddToViewport(UI_LAYER_ALWAYS_TOP);
	PlayerCharacter->GetController<ALLL_PlayerController>()->SetUIInputMode(PermanentEnhancementWidget->GetCachedWidget());
	PermanentEnhancementWidget->SetFocus();
}

void ALLL_WorldTreeObject::SetUpWidgetInfo(ULLL_PermanentEnhancementWidget& PermanentEnhancementWidget)
{
	// 영구강화 관련 UI 나오면 영구강화 데이터 연동시키기
}

