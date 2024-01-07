// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/LLL_MonsterBase.h"

#include "DataAsset/LLL_MonsterDataAsset.h"
#include "Entity/Character/Monster/LLL_MonsterBaseAIController.h"

ALLL_MonsterBase::ALLL_MonsterBase()
{
	// 몬스터 데이터 에셋 할당
	static ConstructorHelpers::FObjectFinder<ULLL_MonsterDataAsset> DA_Monster(TEXT("/Script/Y2024Q1_Prototyping.LLL_MonsterDataAsset'/Game/DataAsset/DA_Monster.DA_Monster'"));
	if (DA_Monster.Succeeded())
	{
		MonsterDataAsset = DA_Monster.Object;
	}

	// AI Controller 할당
	AIControllerClass = ALLL_MonsterBaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

float ALLL_MonsterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CharacterHealthAmount <= 0)
	{
		Dead();
	}
	else
	{
		Stun();
	}
	
	return 0;
}

void ALLL_MonsterBase::Stun()
{
	// Todo: 스턴 애니메이션 재생
	
	bIsStun = true;
}
