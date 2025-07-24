// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBExtensionHubDetailWindow.h"
#include "HUDs/GSBHubDetail.h"
#include "Facility/Facility.h"
#include "Characters/GSBPlayer.h"
#include "Interfaces/HubFacility.h"
#include "DebugHeader.h"

void UGSBExtensionHubDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	if (IHubFacility* Hub = Cast<IHubFacility>(Facility))
	{
		HubDetail->LinkHubStorageComponent(Hub->GetHubStorageComponent());
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("IHubFacility를 상속한 클래스가 아닙니다."));
	}

	if (AGSBPlayer* Player = Cast<AGSBPlayer>(GetOwningPlayerPawn()))
	{
		HubDetail->LinkInventoryComponent(Player->GetInventoryComponent());
	}
}
