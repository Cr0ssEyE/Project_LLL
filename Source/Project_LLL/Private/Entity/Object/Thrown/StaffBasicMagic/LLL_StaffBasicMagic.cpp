// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/Object/Thrown/StaffBasicMagic/LLL_StaffBasicMagic.h"

#include "Constant/LLL_FilePath.h"
#include "DataAsset/LLL_StaffBasicMagicDataAsset.h"
#include "GAS/Attribute/Object/Thrown/StaffBasicMagic/LLL_StaffBasicMagicAttributeSet.h"
#include "Util/LLL_ConstructorHelper.h"

ALLL_StaffBasicMagic::ALLL_StaffBasicMagic()
{
	BaseObjectDataAsset = FLLL_ConstructorHelper::FindAndGetObject<ULLL_StaffBasicMagicDataAsset>(PATH_STAFF_BASIC_MAGIC_DATA, EAssertionLevel::Check);

	ThrownObjectAttributeSet = CreateDefaultSubobject<ULLL_StaffBasicMagicAttributeSet>(TEXT("StaffBasicMagicAttributeSet"));
}
