// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBCentralHubDetailWindow.h"
#include "HUDs/GSBHubDetail.h"
#include "Facility/CentralHub.h"
#include "Characters/GSBPlayer.h"
#include "Interfaces/HubFacility.h"
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

	if (AGSBPlayer* Player = Cast<AGSBPlayer>(GetOwningPlayerPawn()))
	{
		HubDetail->LinkInventoryComponent(Player->GetInventoryComponent());
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("AGSBPlayer캐스팅 실패"));
	}
}
