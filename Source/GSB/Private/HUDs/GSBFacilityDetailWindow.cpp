// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBFacilityDetailWindow.h"
#include "HUDs/GSBFacilityName.h"
#include "Facility/Facility.h"

void UGSBFacilityDetailWindow::OnClosed()
{
	UnlinkFacility();
	Super::OnClosed();
}

void UGSBFacilityDetailWindow::SetFacilityName(const FText InFacilityName)
{
	FacilityName->SetFacilityName(InFacilityName);
}

void UGSBFacilityDetailWindow::UnlinkFacility()
{
	if (IsValid(LinkedFacility))
	{
		LinkedFacility->UnlinkFacilityDetailWindow();
	}
}

void UGSBFacilityDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	LinkedFacility = Facility;
	if (IsValid(LinkedFacility))
	{
		SetFacilityName(LinkedFacility->GetFacilityName());
	}
}

void UGSBFacilityDetailWindow::OnUnlinkedFromFacility()
{
	LinkedFacility = nullptr;
}
