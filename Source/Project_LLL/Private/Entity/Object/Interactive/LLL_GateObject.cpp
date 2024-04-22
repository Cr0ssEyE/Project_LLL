// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/LLL_GateObject.h"

#include "Constant/LLL_FilePath.h"
#include "System/Reward/LLL_RewardGimmick.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateMesh = FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_GATE_OBJECT_TEST_MESH, EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(GateMesh);
	bIsGateEnabled = false;
}

void ALLL_GateObject::SetGateInformation(FTestRewardDataTable* Data)
{
	RewardData = Data;
	
	//TODO: gate의 보상 표시 변경
	switch (RewardData->RewardType)
	{
	case ERewardType::Ability :
		break;
	case ERewardType::MaxHealth :
		break;
	case ERewardType::Gold :
		break;
	default:
		break;
	}
	
	//TODO: 어려움 추가 보상 관련 로직(혹시 몰라서 추가해둠)
	if (RewardData->bIsHardReward)
	{
		
	}
}

void ALLL_GateObject::InteractiveEvent()
{
	if(!bIsGateEnabled)
	{
		return;
	}
	
	Super::InteractiveEvent();
	
	OpenGate();
}

void ALLL_GateObject::OpenGate()
{
	//문 오픈 애니 및 이펙트
	AddActorLocalRotation(FRotator(0.0f, -90.0f, 0.0f));

	FTimerHandle StageDestroyTimerHandle;
	
	GetWorld()->GetTimerManager().SetTimer(StageDestroyTimerHandle, this, &ALLL_GateObject::StartDestroy, 0.1f, false, 0.5f);
}

void ALLL_GateObject::StartDestroy()
{
	GateInteractionDelegate.Broadcast(RewardData);
}
