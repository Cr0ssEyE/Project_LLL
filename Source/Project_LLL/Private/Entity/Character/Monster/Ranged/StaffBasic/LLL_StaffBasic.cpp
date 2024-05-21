// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/Ranged/StaffBasic/LLL_StaffBasic.h"

#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_Monster_Id.h"
#include "Entity/Character/Monster/Ranged/StaffBasic/LLL_StaffBasicAIController.h"
#include "GAS/Attribute/Character/Monster/RangedMonster/StaffBasic/LLL_StaffBasicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_StaffBasic::ALLL_StaffBasic()
{
	CharacterDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_StaffBasicDataAsset>(PATH_STAFF_BASIC_DATA, EAssertionLevel::Check);
	
	StaffBasicAttributeSet = CreateDefaultSubobject<ULLL_StaffBasicAttributeSet>(TEXT("StaffBasicAttributeSet"));
	AIControllerClass = ALLL_StaffBasicAIController::StaticClass();

	Id = ID_STAFF_BASIC;

	// Todo : 어빌리티 작업이 끝난 후 커브 데이터로 옮기기
	InitEffect = FLLL_ConstructorHelper::FindAndGetClass<UGameplayEffect>(TEXT("/Script/Engine.Blueprint'/Game/GAS/Effects/Character/Monster/RangedMonster/StaffBasic/BPGE_StaffBasic_Attribute_Initialize.BPGE_StaffBasic_Attribute_Initialize_C'"), EAssertionLevel::Check);
}

void ALLL_StaffBasic::BeginPlay()
{
	Super::BeginPlay();

	StaffBasicDataAsset = Cast<ULLL_StaffBasicDataAsset>(RangedMonsterDataAsset);

	// Todo : 어빌리티 작업이 끝난 후 커브 데이터로 옮기기
	if (IsValid(InitEffect))
	{
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitEffect, 1.0, EffectContextHandle);
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}
