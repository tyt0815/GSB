// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBChainDeconstructionButton.h"
#include "Components/Button.h"

void UGSBChainDeconstructionButton::NativeConstruct()
{
	ChainDeconstructButton->OnClicked.AddDynamic(this, &UGSBChainDeconstructionButton::DeconstructConnectedConveyorChain);
}

void UGSBChainDeconstructionButton::DeconstructConnectedConveyorChain()
{
	if (IsValid(ChainDeconstrutableFacility.GetObject()))
	{
		ChainDeconstrutableFacility->DeconstructConnectedConveyorChain();
	}
}
