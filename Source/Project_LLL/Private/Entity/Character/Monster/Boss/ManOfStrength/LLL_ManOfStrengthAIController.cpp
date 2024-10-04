// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Boss/ManOfStrength/LLL_ManOfStrengthAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Constant/LLL_BlackBoardKeyNames.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"

void ALLL_ManOfStrengthAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(BlackboardComponent->GetValueAsObject(BBKEY_OTHER_MONSTER)))
	{
		ALLL_MonsterBase* OtherMonster = CastChecked<ALLL_MonsterBase>(BlackboardComponent->GetValueAsObject(BBKEY_OTHER_MONSTER));
		if (OtherMonster->CheckCharacterIsDead())
		{
			BlackboardComponent->SetValueAsObject(BBKEY_OTHER_MONSTER, nullptr);
		}
	}
}

