// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Skill/PlayerFeatherStorm/LLL_PlayerFeatherStorm.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Game/ProtoGameInstance.h"
#include "GAS/Attribute/Object/Skill/PlayerFeatherStorm/LLL_PlayerFeatherStormAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_PlayerFeatherStorm::ALLL_PlayerFeatherStorm()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_PlayerFeatherStormDataAsset>(PATH_PLAYER_FEATHER_STORM_DATA, EAssertionLevel::Check);

	SkillObjectAttributeSet = CreateDefaultSubobject<ULLL_PlayerFeatherStormAttributeSet>(TEXT("PlayerFeatherStormAttributeSet"));
}

void ALLL_PlayerFeatherStorm::BeginPlay()
{
	Super::BeginPlay();

	FeatherStormDataAsset = Cast<ULLL_PlayerFeatherStormDataAsset>(SkillObjectDataAsset);
	
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
