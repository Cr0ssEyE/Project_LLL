// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_AttributeInitializeGroupName.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_GeneralConstants.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseUIManager.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
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

	FTimerHandle SpringArmDistanceTimerHandle;
	GetWorldTimerManager().SetTimer(SpringArmDistanceTimerHandle, FTimerDelegate::CreateWeakLambda(this, [=, this]{
		ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
		const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));

		ChangePlayerOrthoWidth(Player->GetCameraDataAsset()->CameraDistanceWhenBossExist);
	}), 0.2f, false);
}

void ALLL_BossMonster::Dead()
{
	CharacterUIManager->GetCharacterStatusWidget()->SetVisibility(ESlateVisibility::Hidden);
	ChangePlayerOrthoWidth(OriginPlayerOrthoWidth);
	
	Super::Dead();
}

void ALLL_BossMonster::ChangePlayerOrthoWidth(float OrthoWidth) const
{
	ALLL_MonsterBaseAIController* MonsterAIController = CastChecked<ALLL_MonsterBaseAIController>(GetController());
	const ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(MonsterAIController->GetBlackboardComponent()->GetValueAsObject(BBKEY_PLAYER));

	if (FMath::IsNearlyEqual(Player->GetCamera()->OrthoWidth, OrthoWidth, 5.f))
	{
		Player->GetCamera()->SetOrthoWidth(OrthoWidth);
		return;
	}

	Player->GetCamera()->SetOrthoWidth(Player->GetCamera()->OrthoWidth + (Player->GetCamera()->OrthoWidth > OrthoWidth ? -5.f : 5.f));
	
	GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this, [=, this]{
		ChangePlayerOrthoWidth(OrthoWidth);
	}));
}
