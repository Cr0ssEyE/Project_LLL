// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Skill/LLL_Player_Deer_PhysicalProof.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Skill/LLL_PhysicalProofAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_Player_Deer_PhysicalProof::ALLL_Player_Deer_PhysicalProof()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerPhysicalProofDataAsset>(PATH_PLAYER_PHYSICAL_PROOF_DATA, EAssertionLevel::Check);

	SkillObjectAttributeSet = CreateDefaultSubobject<ULLL_PhysicalProofAttributeSet>(TEXT("PlayerFeatherStormAttributeSet"));
}

void ALLL_Player_Deer_PhysicalProof::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
}

void ALLL_Player_Deer_PhysicalProof::BeginPlay()
{
	Super::BeginPlay();

	FeatherStormDataAsset = Cast<ULLL_PlayerPhysicalProofDataAsset>(SkillObjectDataAsset);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FeatherStormDataAsset->Particle, GetActorLocation(), GetActorRotation());

#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
	if (const UProtoGameInstance* ProtoGameInstance = Cast<UProtoGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (ProtoGameInstance->CheckPlayerDashDebug())
		{
			DrawDebugBox(GetWorld(), GetActorLocation(), SkillCollisionBox->GetScaledBoxExtent(), FColor::Blue, false, SkillObjectAttributeSet->GetDestroyTimer());
		}
	}
#endif
}
