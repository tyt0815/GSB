// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/DroppedItem.h"
#include "Items/ItemDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/InteractionComponent.h"
#include "Interfaces/ItemPickupActor.h"
#include "NiagaraComponent.h"

#define DEFAULT_ITEM_NAME TEXT("DroppedItem")

ADroppedItem::ADroppedItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemOverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemOverlap"));
	SetRootComponent(ItemOverlapBox);
	ItemOverlapBox->SetCollisionProfileName(TEXT("PickupItem"));
	ItemOverlapBox->SetSimulatePhysics(true);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
	InteractionComponent->SetupAttachment(GetRootComponent());

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	NiagaraComponent->SetupAttachment(GetRootComponent());

	ItemStack.Stack = 1;
}

void ADroppedItem::GetInteractionDescriptions(TArray<FString>& Descriptions)
{
	InteractionComponent->GetInteractionDescriptions(Descriptions);
}

void ADroppedItem::Interaction(int32 Index, AActor* Interactor)
{
	InteractionComponent->Interaction(Index, Interactor);
}

int32 ADroppedItem::GetNumInteractions() const
{
	return InteractionComponent->GetNumInteractions();
}

bool ADroppedItem::IsInteractable() const
{
	return InteractionComponent->IsInteractable();
}

void ADroppedItem::SetHighlighInteractableActor(bool bHighlight)
{
	if (bHighlight)
	{
		NiagaraComponent->SetAsset(HighlightedNiagara);
	}
	else
	{
		NiagaraComponent->SetAsset(DefaultNiagara);
	}
}

void ADroppedItem::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->AddInteractionDynamic(GetItemName().ToString(), this, &ADroppedItem::HandlePickup);
	UpdateItem(ItemStack);

	DefaultNiagara = NiagaraComponent->GetAsset();
}

void ADroppedItem::UpdateItem(const FItemStack& NewItemStack)
{
	if (NewItemStack.Stack == 0 || !IsValid(NewItemStack.ItemData))
	{
		Destroy();
		return;
	}

	int32 InteractionIndex = InteractionComponent->GetInteractionIndex(GetItemName().ToString());

	ItemStack = NewItemStack;

	ItemName = FName(ItemStack.ItemData->Name.ToString() + FString::Printf(TEXT(" (%d)"), ItemStack.Stack));

	InteractionComponent->SetDescriptionAt(InteractionIndex, ItemName.ToString());
}

void ADroppedItem::HandlePickup(AActor* Interactor)
{
	if (IsValid(Interactor) && Interactor->Implements<UItemPickupActor>())
	{
		if (IItemPickupActor* ItemPickupActor = Cast<IItemPickupActor>(Interactor))
		{
			ItemStack.Stack -= ItemPickupActor->PickupItem(ItemStack);
			UpdateItem(ItemStack);
		}
	}
}
