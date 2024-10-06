// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Character/Monster/DPSTester/LLL_DPSTester.h"

#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_MeshSocketName.h"
#include "DataAsset/Global/LLL_GlobalNiagaraDataAsset.h"
#include "Game/LLL_GameInstance.h"
#include "UI/Entity/Character/Monster/LLL_DPSTesterWidget.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_DPSTester::ALLL_DPSTester()
{
	ResetTimer = 3.0f;
	TotalDamage = 0.0f;
	StartTime = 0.0f;
	
	GetCapsuleComponent()->SetCapsuleSize(100.0f, 200.0f);

	DPSTesterWidgetClass = FLLL_ConstructorHelper::FindAndGetClass<ULLL_DPSTesterWidget>(PATH_UI_DPS_TESTER_WIDGET, EAssertionLevel::Check);
	DPSTesterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DPSTesterWidget"));

	DPSTesterWidgetComponent->SetupAttachment(RootComponent);
	
	GetMesh()->SetSkeletalMesh(FLLL_ConstructorHelper::FindAndGetObject<USkeletalMesh>(PATH_DPS_TESTER_MESH, EAssertionLevel::Check));
}

void ALLL_DPSTester::BeginPlay()
{
	Super::Super::Super::BeginPlay();

	DPSTesterWidget = CastChecked<ULLL_DPSTesterWidget>(CreateWidget(GetWorld(), DPSTesterWidgetClass));

	DPSTesterWidgetComponent->SetWidget(DPSTesterWidget);
	DPSTesterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DPSTesterWidgetComponent->SetRelativeLocation(FVector(400.0f, 400.0f, 0.0f));
	DPSTesterWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	FString DPSText;
	DPSText.Append(TEXT("<ProperNoun>누적 데미지:</>\n"));
	DPSText.Append(TEXT("<ProperNoun>총합 데미지:</>\n"));
	DPSText.Append(TEXT("<ProperNoun>총 데미지 합산 시간:</>\n"));
	DPSText.Append(TEXT("<ProperNoun>DPS:</>"));
	DPSTesterWidget->SetWidgetText(DPSText);
	
	UNiagaraSystem* StackCountNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->StackCountNiagaraSystem;
	if (IsValid(StackCountNiagaraSystem))
	{
		StackVFXComponent->SetAsset(StackCountNiagaraSystem);
		StackVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_OVERHEAD);
	}

	UNiagaraSystem* BleedingNiagaraSystem = GetWorld()->GetGameInstanceChecked<ULLL_GameInstance>()->GetGlobalNiagaraDataAsset()->BleedingNiagaraSystem;
	if (IsValid(BleedingNiagaraSystem))
	{
		BleedingVFXComponent->SetAsset(BleedingNiagaraSystem);
		BleedingVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_CHEST);
	}
}

void ALLL_DPSTester::Damaged(AActor* Attacker, bool IsDOT, float Damage)
{
	Super::Damaged(Attacker, IsDOT, Damage);
	
	if (StartTime == 0.0f)
	{
		StartTime = GetWorld()->GetTimeSeconds();
	}
	
	TotalDamage += Damage;
	
	FString DPSMeasuringText;
	DPSMeasuringText.Append(FString::Printf(TEXT("<ProperNoun>누적 데미지:</> <synergy>%.2f</>\n"), TotalDamage));
	DPSMeasuringText.Append(TEXT("<ProperNoun>총합 데미지:</> 측정 중...\n"));
	DPSMeasuringText.Append(TEXT("<ProperNoun>총 데미지 합산 시간:</> 측정 중...\n"));
	DPSMeasuringText.Append(TEXT("<ProperNoun>DPS:</> 측정 중..."));
	DPSTesterWidget->SetWidgetText(DPSMeasuringText);

	GetWorldTimerManager().ClearTimer(TotalDamageTimerHandle);
	GetWorldTimerManager().SetTimer(TotalDamageTimerHandle, FTimerDelegate::CreateWeakLambda(this, [&]{
		const float EndTime = GetWorld()->GetTimeSeconds();
		const float TotalElapsedTime = EndTime - StartTime - ResetTimer;
		
		FString DPSResultText;
		DPSResultText.Append(TEXT("<ProperNoun>누적 데미지:</>\n"));
		DPSResultText.Append(FString::Printf(TEXT("<ProperNoun>총합 데미지:</> <status>%.2f</>\n"), TotalDamage));
		if (FMath::IsNearlyEqual(TotalElapsedTime, 0.0f, 0.1f))
		{
			DPSResultText.Append(TEXT("<ProperNoun>총 데미지 합산 시간:</> <CriticalDamage>측정 불가</>\n"));
			DPSResultText.Append(FString::Printf(TEXT("<ProperNoun>DPS:</> <status>%.2f</>"), TotalDamage));
		}
		else
		{
			DPSResultText.Append(FString::Printf(TEXT("<ProperNoun>총 데미지 합산 시간:</> <status>%.2f</><ProperNoun>초</>\n"), TotalElapsedTime));
			DPSResultText.Append(FString::Printf(TEXT("<ProperNoun>DPS:</> <status>%.2f</>"), TotalDamage / TotalElapsedTime));
		}
		DPSTesterWidget->SetWidgetText(DPSResultText);
		
		TotalDamage = 0.0f;
		StartTime = 0.0f;
	}), ResetTimer, false);
}
