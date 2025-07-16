// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityName.h"
#include "Components/TextBlock.h"

void UGSBFacilityName::SetFacilityName(const FText InFacilityName)
{
	FacilityName->SetText(InFacilityName);
}
