// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HoveredItemName.h"
#include "Components/TextBlock.h"

void UHoveredItemName::SetItemName(const FText& NewItemName)
{
	if (IsValid(ItemName))
	{
		ItemName->SetText(NewItemName);
	}
}
