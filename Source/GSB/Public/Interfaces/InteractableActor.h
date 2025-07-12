// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GSB_API IInteractableActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) = 0;

	virtual void Interaction(int32 Index, AActor* Interactor) = 0;

	virtual int32 GetNumInteractions() const = 0;

	virtual bool IsInteractable() const = 0;

	virtual void SetHighlighInteractableActor(bool bHighlight) = 0;
};
