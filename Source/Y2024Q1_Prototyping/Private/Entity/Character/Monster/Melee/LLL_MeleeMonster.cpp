// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Melee/LLL_MeleeMonster.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_MeleeMonsterDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBaseAnimInstance.h"
#include "Entity/Character/Monster/Melee/LLL_MeleeMonsterAIController.h"
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
	
	if(IsValid(CharacterAnimInstance))
	{
		CharacterAnimInstance->SetDataAsset(CharacterDataAsset);
	}
}

bool ALLL_MeleeMonster::AttackAnimationIsPlaying()
{
	Super::AttackAnimationIsPlaying();

	if (IsValid(CharacterAnimInstance))
	{
		return CharacterAnimInstance->Montage_IsPlaying(CharacterDataAsset->AttackAnimMontage);
	}

	return false;
}

void ALLL_MeleeMonster::DamageToPlayer()
{
	// Todo: 플레이어에게 데미지 구현
}
