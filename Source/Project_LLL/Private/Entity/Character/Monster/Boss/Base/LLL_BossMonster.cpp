// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"

#include "Components/CapsuleComponent.h"
#include "Constant/LLL_AttributeInitializeGroupName.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GeneralConstants.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"
#include "UI/Entity/Character/Monster/LLL_BossStatusWidget.h"

ALLL_BossMonster::ALLL_BossMonster()
{
	AIControllerClass = ALLL_BossMonsterAIController::StaticClass();

	AttributeInitId = ATTRIBUTE_INIT_BOSS;

	GetMesh()->SetCollisionProfileName(CP_BOSS);
	GetCapsuleComponent()->SetCollisionProfileName(CP_BOSS);
}

void ALLL_BossMonster::BeginPlay()
{
	Super::BeginPlay();

	BossMonsterDataAsset = Cast<ULLL_BossMonsterDataAsset>(MonsterBaseDataAsset);
	
	CurrentHavePatterns = BossMonsterDataAsset->DefaultPatterns;
	CharacterUIManager->GetCharacterStatusWidget()->AddToViewport(UI_LAYER_THIRD);
}

void ALLL_BossMonster::Dead()
{
	CharacterUIManager->GetCharacterStatusWidget()->SetVisibility(ESlateVisibility::Hidden);
	Super::Dead();
}
