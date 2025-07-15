// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBTransportFacilityDetailWindow.h"
#include "HUDs/GSBChainDeconstructionButton.h"
#include "Facility/Facility.h"

void UGSBTransportFacilityDetailWindow::OnLinkedToFacility(AFacility* Facility)
{
	Super::OnLinkedToFacility(Facility);
	ChainDeconstructButton->SetChainDeconstrutableFacility(TScriptInterface<IChainDeconstrutableFacility>(Facility));
}
