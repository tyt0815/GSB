// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemCrate.h"
#include "Items/DroppedItem.h"

// Sets default values
AItemCrate::AItemCrate()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crate"));
}

ADroppedItem* AItemCrate::ConvertToDroppedItem()
{
	ADroppedItem* DroppedItem = ADroppedItem::CreateDroppedItem(this, ItemData, GetActorLocation());
	Destroy();
	return DroppedItem;
}
