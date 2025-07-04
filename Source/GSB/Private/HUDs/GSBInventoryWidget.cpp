// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInventoryWidget.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UGSBInventoryWidget::UpdateWidget()
{
	Super::UpdateWidget();

	if (UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(LinkedItemStorageComponent))
	{
		int32 MaxWeight = InventoryComponent->GetMaxWeight();
		int32 CurrentWeight = InventoryComponent->GetCurrentWeight();

		WeightLimitText->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"), MaxWeight, CurrentWeight)));
		WeightProgressBar->SetPercent(static_cast<float>(CurrentWeight) / MaxWeight);
	}
}
