// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInteractionDescriptionWidget.h"
#include "Components/TextBlock.h"

void UGSBInteractionDescriptionWidget::SetDescription(const FText& NewDescription)
{
	Description->SetText(NewDescription);
}
