// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"

#include "Constant/LLL_FilePath.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateMesh = FLLL_ConstructorHelper::FindAndGetObject<UStaticMesh>(PATH_GATE_OBJECT_TEST_MESH, EAssertionLevel::Check);
	BaseMesh->SetStaticMesh(GateMesh);
	bIsGateEnabled = false;
}

void ALLL_GateObject::SetGateInformation(FRewardDataTable* Data)
{
	RewardData = Data;
	
	//TODO: gate의 보상 표시 변경
	//데이터가 현재 확정되지 않아 임시로 Enum을 배정해서 사용중
	switch (RewardData->ID)
	{
	case static_cast<int>(ERewardCategory::Gold):
		break;
	case static_cast<int>(ERewardCategory::Ability):
		//능력의 경우 현재 보상 데이터 테이블에서 어떤 동물의 능력인지 구분할 수 없어 임의로 코드 작성함
		AbilityType = static_cast<EAbilityType>(FMath::RandRange(1, 3));
		break;
	case static_cast<int>(ERewardCategory::Enhance):
		break;
	case static_cast<int>(ERewardCategory::MaxHP):
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
