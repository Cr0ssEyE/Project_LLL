// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/EnergyBlasterEnergy/LLL_EnergyBlasterEnergy.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_EnergyBlasterEnergyDataAsset.h"
#include "GAS/Attribute/Object/ThrownObject/EnergyBlasterEnergy/LLL_EnergyBlasterEnergyAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_EnergyBlasterEnergy::ALLL_EnergyBlasterEnergy()
{
	BaseObjectDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_EnergyBlasterEnergyDataAsset>(PATH_ENERGY_BLASTER_ENERGY_DATA, EAssertionLevel::Check);

	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_EnergyBlasterEnergyAttributeSet>(TEXT("EnergyBlasterEnergyAttributeSet"));
}
