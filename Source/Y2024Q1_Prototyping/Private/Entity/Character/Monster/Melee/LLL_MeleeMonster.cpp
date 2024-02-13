// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Engine/DamageEvents.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAIController.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/ProtoGameInstance.h"
#include "Util/LLLConstructorHelper.h"

ALLL_MeleeMonster::ALLL_MeleeMonster()
{
	// 몬스터 데이터 에셋 할당
	CharacterDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_MeleeMonsterDataAsset>(PATH_MELEE_MONSTER_DATA, EAssertionLevel::Check);
	MeleeMonsterDataAsset = Cast<ULLL_MeleeMonsterDataAsset>(CharacterDataAsset);
	if (IsValid(MeleeMonsterDataAsset))
	{
		DetectDistance = MeleeMonsterDataAsset->DetectDistance;
		FieldOfView = MeleeMonsterDataAsset->FieldOfView;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MeleeMonsterAIController::StaticClass();
}

void ALLL_MeleeMonster::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(CharacterAnimInstance = Cast<ULLL_BaseCharacterAnimInstance>(GetMesh()->GetAnimInstance())))
	{
		CharacterAnimInstance->SetDataAsset(CharacterDataAsset);
	}
}
