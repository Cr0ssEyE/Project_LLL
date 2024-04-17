// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"

#include "Blueprint/UserWidget.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "UI/LLL_CharacterStatusWidget.h"

// Sets default values for this component's properties
ULLL_MonsterBaseUIManager::ULLL_MonsterBaseUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void ULLL_MonsterBaseUIManager::BeginPlay()
{
	Super::BeginPlay();

	const ULLL_BaseCharacterDataAsset* CharacterDataAsset = CastChecked<ALLL_MonsterBase>(GetOwner())->GetCharacterDataAsset();
	CharacterStatusWidgetClass = CharacterDataAsset->StatusWidgetClass;
	CharacterStatusWidget = CastChecked<ULLL_CharacterStatusWidget>(CreateWidget(GetWorld(), CharacterStatusWidgetClass));
}


// Called every frame
void ULLL_MonsterBaseUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
