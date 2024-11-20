// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Breakable/LLL_BreakableObjectBase.h"

#include "FMODAudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "Entity/Character/Monster/Base/LLL_MonsterBase.h"
#include "Entity/Character/Monster/Boss/Base/LLL_BossMonster.h"
#include "Entity/Character/Monster/DPSTester/LLL_DPSTester.h"
#include "Game/LLL_DebugGameInstance.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_BreakableObjectBase::ALLL_BreakableObjectBase()
{
	GlobalNiagaraDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_GlobalNiagaraDataAsset>(PATH_GLOBAL_NIAGARA_EFFECT_DATA, EAssertionLevel::Check);
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleComponent->SetCollisionProfileName(CP_MONSTER);
	//SetRootComponent(CapsuleComponent);
	CapsuleComponent->SetupAttachment(RootComponent);

	//BaseMesh->SetupAttachment(RootComponent);
	FModAudioComponent->SetupAttachment(RootComponent);

	Crack = 0;
}

void ALLL_BreakableObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
	//CapsuleComponent->SetCollisionProfileName(CP_MONSTER);
	//SetRootComponent(CapsuleComponent);
}

void ALLL_BreakableObjectBase::ReceivePlayerAttackOrKnockBackedMonster()
{
	if (Crack < 2)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GlobalNiagaraDataAsset->StatueDestroyNiagaraSystem, GetActorLocation());
		
		Crack++;
		UE_LOG(LogTemp, Log, TEXT("조각상 때림 : %d대"), Crack)
	}
	else if (Crack == 2)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GlobalNiagaraDataAsset->StatueDestroyNiagaraSystem, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), GlobalNiagaraDataAsset->StatueDestroyWithEnergyNiagaraSystem, GetActorLocation());

		TArray<FHitResult> HitResults;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
	
		GetWorld()->SweepMultiByChannel(
			HitResults,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECC_ENEMY,
			FCollisionShape::MakeSphere(StunRadius),
			Params
			);

		FColor DebugColor = FColor::Red;
		for (auto HitResult : HitResults)
		{
			ALLL_MonsterBase* OtherMonster = Cast<ALLL_MonsterBase>(HitResult.GetActor());
			if (IsValid(OtherMonster) && !OtherMonster->CheckCharacterIsDead() && !Cast<ALLL_DPSTester>(OtherMonster) && !Cast<ALLL_BossMonster>(OtherMonster))
			{
				OtherMonster->Stun();
				DebugColor =  FColor::Green;
			}
		}

		/*for (USceneComponent* ChildComponent : CapsuleComponent->GetAttachChildren())
		{
			UE_LOG(LogTemp, Log, TEXT("%d"), CapsuleComponent->GetAttachChildren().Num())
			const UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ChildComponent);
			if (IsValid(StaticMeshComponent))
			{*/
				UStaticMesh* StaticMesh = BaseMesh->GetStaticMesh();
				UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this);
				if (IsValid(Material))
				{
					StaticMesh->SetMaterial(0, Material);
					UE_LOG(LogTemp, Log, TEXT("%s"), *Material->GetName())
					Material->SetTextureParameterValue(TEXT("Albedo"), DestroyedTexture);
					Material->SetTextureParameterValue(TEXT("Opacity Mask"), DestroyedTexture);
				}
			//}
		//}
		
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckPlayerAttackDebug())
			{
				DrawDebugSphere(GetWorld(), GetActorLocation(), StunRadius, 16, DebugColor, false, 2.0f);
			}
		}
#endif
		
		Crack++;
		UE_LOG(LogTemp, Log, TEXT("조각상 때림 : %d대"), Crack)
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("조각상 다 때림"))
	}
}
