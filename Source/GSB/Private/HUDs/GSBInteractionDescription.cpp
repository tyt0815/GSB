// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInteractionDescription.h"
#include "Components/TextBlock.h"

void UGSBInteractionDescription::SetDescription(const FText& NewDescription)
{
	Description->SetText(NewDescription);
}
