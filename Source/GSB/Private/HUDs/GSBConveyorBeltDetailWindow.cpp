// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBConveyorBeltDetailWindow.h"
#include "Components/Button.h"
#include "Facility/ConveyorBelt.h"

void UGSBConveyorBeltDetailWindow::NativeConstruct()
{
	Super::NativeConstruct();

	ChainDeconstructButton->OnClicked.AddDynamic(this, &UGSBConveyorBeltDetailWindow::DeconstructConnectedConveyorChain);
}

void UGSBConveyorBeltDetailWindow::DeconstructConnectedConveyorChain()
{
	if (AConveyorBelt* ConveyorBelt = Cast<AConveyorBelt>(LinkedFacility))
	{
		ConveyorBelt->DeconstructConnectedConveyorChain();
	}
	Close();
}
