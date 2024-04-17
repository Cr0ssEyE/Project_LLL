// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/EnergyBlasterEnergy/LLL_EnergyBlasterEnergy.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_EnergyBlasterEnergyDataAsset.h"
#include "GAS/Attribute/Object/ThrownObject/EnergyBlasterEnergy/LLL_EnergyAttributeSet.h"
#include "Util/LLLConstructorHelper.h"

ALLL_EnergyBlasterEnergy::ALLL_EnergyBlasterEnergy()
{
	BaseObjectDataAsset = FLLLConstructorHelper::FindAndGetObject<ULLL_EnergyBlasterEnergyDataAsset>(PATH_ENERGYBLASTER_ENERGY_DATA, EAssertionLevel::Check);

	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_EnergyAttributeSet>(TEXT("EnergyBlasterEnergyAttributeSet"));
}
