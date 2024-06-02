// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Gate/LLL_GateObject.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_GateDataAsset.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Enumeration/LLL_GameSystemEnumHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GateDataAsset>(PATH_GATE_DATA, EAssertionLevel::Check);
	GateMesh = GateDataAsset->StaticMesh;
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

void ALLL_GateObject::SetActivate()
{
	bIsGateEnabled = true;
	if (IsValid(GateDataAsset->Particle))
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(GateDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), GateDataAsset->ParticleLocation, FRotator::ZeroRotator, GateDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
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

void ALLL_GateObject::BeginPlay()
{
	Super::BeginPlay();
	InteractOnlyCollisionBox->SetBoxExtent(FVector(200.0f, 200.0f, 300.f));
	InteractOnlyCollisionBox->SetRelativeLocation(FVector(0, 0, 300.f));
}

void ALLL_GateObject::OpenGate()
{
	//문 오픈 애니 및 이펙
	FTimerHandle StageDestroyTimerHandle;
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	/*UNiagaraComponent* NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("PlayerTeleportEffect"));
	NiagaraComponent->SetAsset(GateDataAsset->Particle);
	NiagaraComponent->SetWorldLocation(Player->GetActorLocation());
	NiagaraComponent->OnSystemFinished.AddDynamic(this, &ALLL_GateObject::PlayerTeleport);*/
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld() ,GateDataAsset->Particle, Player->GetActorLocation(), FRotator::ZeroRotator, GateDataAsset->ParticleScale);
	
	GetWorld()->GetTimerManager().SetTimer(StageDestroyTimerHandle, this, &ALLL_GateObject::StartDestroy, 0.1f, false, 3.0f);
}

void ALLL_GateObject::StartDestroy()
{
	GateInteractionDelegate.Broadcast(RewardData);
}

void ALLL_GateObject::PlayerTeleport(UNiagaraComponent* NiagaraComponent)
{
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->SetActorHiddenInGame(true);
}
