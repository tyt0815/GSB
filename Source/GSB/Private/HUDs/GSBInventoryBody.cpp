// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBInventoryBody.h"
#include "Components/TextBlock.h"

void UGSBInventoryBody::SetWeight(int32 CurrentWeight, int32 MaxWeight)
{
	Weight->SetText(FText::FromString(FString::Printf(TEXT("(%d/%d)"), CurrentWeight, MaxWeight)));
}
