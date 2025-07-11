// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemReceiver.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemReceiver : public UInterface
{
	GENERATED_BODY()
};



class GSB_API IItemReceiver
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanReceiveItem() const = 0;

	virtual void OnConnectedToItemSender(AActor* NewSender) = 0;

	virtual void OnDisconnectedFromItemSender() = 0;

	virtual void OnReceiveItem(AActor* Item) = 0;

	virtual FVector GetReceivingDirection() const = 0;
};
