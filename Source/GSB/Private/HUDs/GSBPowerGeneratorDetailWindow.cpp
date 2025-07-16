// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBPowerGeneratorDetailWindow.h"
#include "HUDs/GSBItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Facility/PowerGenerator.h"

void UGSBPowerGeneratorDetailWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APowerGenerator* PowerGenerator = Cast<APowerGenerator>(LinkedFacility))
	{
		SetRemainingPowerTimeProgressBar(PowerGenerator->GetRemainingPowerTimeProgress());
	}
}

void UGSBPowerGeneratorDetailWindow::SetConsumingItem(const FText& ConsumingItemName)
{
	ConsumingItem->SetText(ConsumingItemName);
}

void UGSBPowerGeneratorDetailWindow::SetGeneratingTime(float Seconds)
{
	GeneratingTime->SetText(FText::FromString(FString::SanitizeFloat(Seconds)));
}

void UGSBPowerGeneratorDetailWindow::SetConsumingItemSlot(const FItemStack& ItemStack)
{
	ConsumingItemSlot->SetLabelByItemStack(ItemStack);
}

void UGSBPowerGeneratorDetailWindow::SetRemainingPowerTimeProgressBar(float Progress)
{
	RemainingPowerTimeProgressBar->SetPercent(Progress);
}
