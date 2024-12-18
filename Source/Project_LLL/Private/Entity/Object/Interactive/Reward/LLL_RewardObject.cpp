// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Interactive/Reward/LLL_RewardObject.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "Util/LLL_ConstructorHelper.h"
#include "Components/WidgetComponent.h"
#include "Constant/LLL_CollisionChannel.h"
#include "Constant/LLL_FilePath.h"
#include "Constant/LLL_GraphicParameterNames.h"
#include "DataTable/LLL_RewardDataTable.h"
#include "Entity/Character/Player/LLL_PlayerBase.h"
#include "Game/LLL_DebugGameInstance.h"
#include "UI/Object/LLL_ProductObjectPriceWidget.h"
#include "Entity/Character/Player/LLL_PlayerUIManager.h"
#include "Game/LLL_GameProgressManageSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/System/LLL_SelectRewardWidget.h"

ALLL_RewardObject::ALLL_RewardObject()
{
	RewardObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_RewardObjectDataAsset>(PATH_REWARD_OBJECT_TEST_DATA, EAssertionLevel::Check);

	BaseMesh->SetStaticMesh(RewardObjectDataAsset->RewardTextureMesh);
	BaseMesh->SetMaterial(0, RewardObjectDataAsset->TextureMaterialInst);
	BaseMesh->SetCollisionProfileName(CP_OVERLAP_ALL);

	PriceWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("PriceWidgetComponent");
	PriceWidgetComponent->SetupAttachment(RootComponent);
	PriceWidget = CreateDefaultSubobject<ULLL_ProductObjectPriceWidget>(TEXT("PriceWidget"));

	InteractOnlyCollisionBox->SetBoxExtent(RewardObjectDataAsset->InteractOnlyCollisionBoxExtent);
	
	Price = RewardObjectDataAsset->Price;
}

void ALLL_RewardObject::BeginPlay()
{
	Super::BeginPlay();
	
	PriceWidgetClass = RewardObjectDataAsset->PriceWidgetClass;
	PriceWidget = CastChecked<ULLL_ProductObjectPriceWidget>(CreateWidget(GetWorld(), PriceWidgetClass));
	
	PriceWidgetComponent->SetWidget(PriceWidget);
	PriceWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PriceWidgetComponent->SetRelativeLocation(RewardObjectDataAsset->PriceWidgetLocation);
	PriceWidgetComponent->SetDrawSize(RewardObjectDataAsset->PriceWidgetSize);
	PriceWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PriceWidget->SetVisibility(ESlateVisibility::Hidden);
	PriceWidget->SetIsEnabled(false);

	if (bIsProduct)
	{
		ApplyProductEvent();
	}
	SetActorScale3D(FVector(2.0f,2.0f,2.0f));

	if (IsValid(RewardObjectDataAsset->Particle))
	{
		AddNiagaraComponent(UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld() ,RewardObjectDataAsset->Particle, GetActorLocation() + RewardObjectDataAsset->ParticleLocation, GetActorRotation(), RewardObjectDataAsset->ParticleScale, false, false));
	}
}

void ALLL_RewardObject::ApplyProductEvent()
{
	bIsProduct = true;

	PriceWidget->SetVisibility(ESlateVisibility::Visible);
	PriceWidget->SetIsEnabled(true);
	PriceWidget->SetPrice(Price);
}

void ALLL_RewardObject::SetInformation(const FRewardDataTable* Data, const uint32 Index)
{
	RewardData = Data;
	RewardIndex = Index;
	
	/*switch (RewardData->ID)
	{
			// 능력
		case 1:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(MAT_PARAM_TEXTURE, RewardObjectDataAsset->AbilityTexture);
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(MAT_PARAM_COLOR, FLinearColor::Green);
		break;
			// 재화
		case 2:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(MAT_PARAM_TEXTURE, RewardObjectDataAsset->GoldTexture);
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(MAT_PARAM_COLOR, FLinearColor::Yellow);
		break;
			// 최대 체력
		case 3:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(MAT_PARAM_TEXTURE, RewardObjectDataAsset->MaxHPTexture);
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(MAT_PARAM_COLOR, FLinearColor::Red);
		break;
			// 능력 강화
		case 4:
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetTextureParameterValue(MAT_PARAM_TEXTURE, RewardObjectDataAsset->EnhanceTexture);
			BaseMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(MAT_PARAM_COLOR, FLinearColor::Blue);
			break;
	default:
		checkNoEntry();
	}*/
}

void ALLL_RewardObject::InteractiveEvent(AActor* InteractedActor)
{
	Super::InteractiveEvent(InteractedActor);
	
	ALLL_PlayerBase* Player = CastChecked<ALLL_PlayerBase>(InteractedActor);
	ULLL_PlayerGoldComponent* PlayerGoldComponent = Player->GetGoldComponent();
	
	FGameplayEffectContextHandle EffectContextHandle = Player->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	EffectContextHandle.AddInstigator(this, this);
	const FGameplayEffectSpecHandle EffectSpecHandle = Player->GetAbilitySystemComponent()->MakeOutgoingSpec(RewardObjectDataAsset->MaxHPEffect, 1.0, EffectContextHandle);
	
	if (bIsProduct && PlayerGoldComponent->GetMoney() < Price)
	{
		//구매 불가능 UI 생성
		return;
	}
	if (bIsProduct)
	{
		PlayerGoldComponent->DecreaseMoney(Price);
	}
	
	if (!RewardData)
	{
		const ULLL_GameInstance* GameInstance = CastChecked<ULLL_GameInstance>(GetWorld()->GetGameInstance());
		TArray<const FRewardDataTable*> RewardDataArray = GameInstance->GetRewardDataTable();
		const uint8 Index = FMath::RandRange(0, RewardDataArray.Num() - 1);
		RewardData = RewardDataArray[Index];
	}

	//ULLL_SelectRewardWidget* SelectRewardWidget = Player->GetPlayerUIManager()->GetSelectRewardWidget();
	/*switch (RewardData->ID)
	{
		// 능력
	case 1:
		InteractionDelegate.Broadcast(this);
		/*SelectRewardWidget->SetVisibility(ESlateVisibility::Visible);
		SelectRewardWidget->SetIsEnabled(true);
		SelectRewardWidget->FocusToUI();#1#
		break;
		// 재화
	case 2:
		PlayerGoldComponent->IncreaseMoney(RewardData->Value);
		break;
		// 최대 체력
	case 3:
		if(EffectSpecHandle.IsValid())
		{
			ASC->BP_ApplyGameplayEffectSpecToTarget(EffectSpecHandle, Player->GetAbilitySystemComponent());
			GetGameInstance()->GetSubsystem<ULLL_GameProgressManageSubSystem>()->GetCurrentSaveGameData()->PlayerPlayProgressData.AcquiredGoldAppleCount++;
		}

		break;
		// 능력 강화
	case 4:
#if (WITH_EDITOR || UE_BUILD_DEVELOPMENT)
		if (const ULLL_DebugGameInstance* DebugGameInstance = Cast<ULLL_DebugGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (DebugGameInstance->CheckObjectActivateDebug())
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("player 어빌리티 강화"));
			}
		}
#endif
		break;
	default:;
	}*/
}
