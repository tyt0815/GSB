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
	if (OnInteractionDelegates.IsValidIndex(Index))
	{
		OnInteractionDelegates[Index].Broadcast(Interactor);
	}
}

int32 UInteractionComponent::GetInteractionIndex(const FString& Description)
{
	for (int32 i = 0; i < InteractionDescriptions.Num(); ++i)
	{
		if (InteractionDescriptions[i] == Description)
		{
			return i;
		}
	}
	return -1;
}

FComponentOnInteractionSignature& UInteractionComponent::AddInteraction_Internal(const FString& Description)
{
	InteractionDescriptions.Add(Description);
	OnInteractionDelegates.Add({});
	return OnInteractionDelegates.Last();
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