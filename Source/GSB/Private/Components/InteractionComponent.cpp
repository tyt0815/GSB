// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"
#include "GSBGameInstance.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(TEXT("InteractionRange"));
}

void UInteractionComponent::Interaction(int32 Index, AActor* Interactor)
{
	if (InteractionData.OnInteractionDelegates.IsValidIndex(Index))
	{
		InteractionData.OnInteractionDelegates[Index].Broadcast(Interactor);
	}
}

void UInteractionComponent::InteractionByDescription(const FString& Description, AActor* Interactor)
{
	Interaction(GetInteractionIndex(Description), Interactor);
}

int32 UInteractionComponent::GetInteractionIndex(const FString& Description)
{
	for (int32 i = 0; i < InteractionData.InteractionDescriptions.Num(); ++i)
	{
		if (InteractionData.InteractionDescriptions[i] == Description)
		{
			return i;
		}
	}
	return -1;
}

FComponentOnInteractionSignature& UInteractionComponent::AddInteraction_Internal(const FString& Description)
{
	InteractionData.InteractionDescriptions.Add(Description);
	InteractionData.OnInteractionDelegates.Add({});
	bInteractionListDirty = true;
	return InteractionData.OnInteractionDelegates.Last();
}

void UInteractionComponent::RemoveInteractionAt(int32 i)
{
	if (InteractionData.InteractionDescriptions.IsValidIndex(i))
	{
		InteractionData.InteractionDescriptions.RemoveAt(i);
		InteractionData.OnInteractionDelegates.RemoveAt(i);
		bInteractionListDirty = true;
	}
}

void UInteractionComponent::ClearInteractions()
{
	for (int i = GetNumInteractions() - 1; i >= 0; --i)
	{
		RemoveInteractionAt(i);
	}
}

void UInteractionComponent::ActivateInteraction()
{
	bInteractable = true;
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UInteractionComponent::DeactivateInteraction()
{
	bInteractable = false;
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
