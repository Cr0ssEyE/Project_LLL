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
#include "Util/LLL_FModPlayHelper.h"
#include "DataAsset/LLL_RewardObjectDataAsset.h"

ALLL_GateObject::ALLL_GateObject()
{
	GateDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GateDataAsset>(PATH_GATE_DATA, EAssertionLevel::Check);
	RewardObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RewardObjectDataAsset>(PATH_REWARD_OBJECT_TEST_DATA, EAssertionLevel::Check);
	GateMesh = GateDataAsset->StaticMesh;
	BaseMesh->SetStaticMesh(GateMesh);

	TextureMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TextureMashComponent"));
	TextureMeshComponent->SetupAttachment(RootComponent);
	TextureMeshComponent->SetRelativeLocation(FVector(60.0f, 60.0f, 370.0f));
	TextureMeshComponent->SetMaterial(0, RewardObjectDataAsset->TextureMaterialInst);
	TextureMeshComponent->SetVisibility(false);

	RewardTextureMesh = RewardObjectDataAsset->RewardTextureMesh;
	TextureMeshComponent->SetStaticMesh(RewardTextureMesh);

	bIsGateEnabled = false;
}

void ALLL_GateObject::SetGateInformation(const FRewardDataTable* Data)
{
	RewardData = Data;
	
	//TODO: gate의 보상 표시 변경
	//데이터가 현재 확정되지 않아 임시로 Enum을 배정해서 사용중
	switch (RewardData->ID)
	{
		// 능력
	case 1:
		TextureMeshComponent->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->AbilityTexture);
		break;
		// 재화
	case 2:
		TextureMeshComponent->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->GoldTexture);
		break;
		// 최대 체력
	case 3:
		TextureMeshComponent->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->MaxHPTexture);
		break;
		// 능력 강화
	case 4:
		TextureMeshComponent->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(TEXT("Texture"), RewardObjectDataAsset->EnhanceTexture);
		break;
	default:;
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
		AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAttached(GateDataAsset->Particle, RootComponent, FName(TEXT("None(Socket)")), GateDataAsset->ParticleLocation, FRotator::ZeroRotator, GateDataAsset->ParticleScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None));
	}
	TextureMeshComponent->SetVisibility(true);
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
	FFModInfo FModInfo;
	FModInfo.FModEvent = GateDataAsset->ActivateEvent;
	FLLL_FModPlayHelper::PlayFModEvent(this, FModInfo);
	FadeOutDelegate.Broadcast();
	FTimerHandle StageDestroyTimerHandle;
	GetWorldTimerManager().SetTimer(StageDestroyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		GateInteractionDelegate.Broadcast(RewardData);
	}), 5.0f, false);
	//문 오픈 애니 및 이펙
}