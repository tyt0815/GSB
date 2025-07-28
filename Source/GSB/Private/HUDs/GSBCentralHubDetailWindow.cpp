// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBCentralHubDetailWindow.h"
#include "HUDs/GSBHubDetail.h"
#include "Facility/CentralHub.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/InventoryInterface.h"
#include "DebugHeader.h"

void UGSBCentralHubDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);

	if (ACentralHub* Hub = Cast<ACentralHub>(Facility))
	{
		HubDetail->LinkHubStorageComponent(Hub->GetHubStorageComponent());
	}
	else
	{ 
		TRACE_SCREEN_LOG(TEXT("ACentralHub 캐스팅 실패"));
	}

	if (IInventoryInterface* InventoryInterface = Cast<IInventoryInterface>(GetOwningPlayerPawn()))
	{
		HubDetail->LinkInventoryComponent(InventoryInterface->GetInventoryComponent());
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("IInventoryInterface캐스팅 실패"));
	}
}
