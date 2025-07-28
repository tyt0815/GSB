// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemCrate.h"
#include "Items/DroppedItem.h"
#include "Items/HoveredItemName.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Characters/GSBPlayer.h"

// Sets default values
AItemCrate::AItemCrate()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetShowBounds = CreateDefaultSubobject<USphereComponent>(TEXT("WidgetShowBounds"));
	SetRootComponent(WidgetShowBounds);
	WidgetShowBounds->OnComponentBeginOverlap.AddDynamic(this, &AItemCrate::OnWidgetShowBoundsBeginOverlap);
	WidgetShowBounds->OnComponentEndOverlap.AddDynamic(this, &AItemCrate::OnWidgetShowBoundsEndOverlap);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crate"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());;

	HoveredItemNameWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HoveredItemNameWidget"));
	HoveredItemNameWidgetComponent->SetupAttachment(GetRootComponent());
}

ADroppedItem* AItemCrate::ConvertToDroppedItem()
{
	ADroppedItem* DroppedItem = ADroppedItem::CreateDroppedItem(this, ItemData, GetActorLocation());
	Destroy();
	return DroppedItem;
}

void AItemCrate::BeginPlay()
{
	Super::BeginPlay();


	HideHoveredItemNameWidget();
	WidgetShowBounds->UpdateOverlaps();
	TArray<AActor*> OverlappedActors;
	WidgetShowBounds->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		if (Actor->GetClass()->IsChildOf<AGSBPlayer>())
		{
			ShowHoveredItemNameWidget();
			break;
		}
	}
}

void AItemCrate::SetItemData(UItemDataAsset* ItemDataAsset)
{
	ItemData = ItemDataAsset;

	if (IsValid(ItemData))
	{
		if (UHoveredItemName* HoveredItemName = Cast<UHoveredItemName>(HoveredItemNameWidgetComponent->GetWidget()))
		{
			HoveredItemName->SetItemName(ItemData->Name);
		}
	}
}

void AItemCrate::OnWidgetShowBoundsBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}
	if (OtherActor->GetClass()->IsChildOf<AGSBPlayer>())
	{
		ShowHoveredItemNameWidget();
	}
}

void AItemCrate::OnWidgetShowBoundsEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}
	if (OtherActor->GetClass()->IsChildOf<AGSBPlayer>())
	{
		HideHoveredItemNameWidget();
	}
}

void AItemCrate::ShowHoveredItemNameWidget()
{
	HoveredItemNameWidgetComponent->SetVisibility(true, true);
}

void AItemCrate::HideHoveredItemNameWidget()
{
	HoveredItemNameWidgetComponent->SetVisibility(false, true);
}
