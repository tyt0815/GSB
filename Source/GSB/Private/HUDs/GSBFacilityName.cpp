// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityName.h"
#include "Components/TextBlock.h"

void UGSBFacilityName::SetFacilityName(const FName& InFacilityName)
{
	FacilityName->SetText(FText::FromString(InFacilityName.ToString()));
}
