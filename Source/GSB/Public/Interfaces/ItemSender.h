// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemSender.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemSender : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GSB_API IItemSender
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool TrySendItem() = 0;

	virtual bool TryConnectItemReceiver(AActor* NewReceiver) = 0;

	virtual void DisconnectItemReceiver() = 0;

	virtual AActor* GetItemToSend() const = 0;
};
