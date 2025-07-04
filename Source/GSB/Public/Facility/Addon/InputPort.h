// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Addon/ItemPort.h"
#include "Interfaces/ItemReceiver.h"
#include "InputPort.generated.h"

class UItemReceiveComponent;

UCLASS()
class GSB_API AInputPort : public AItemPort, public IItemReceiver
{
	GENERATED_BODY()

public:
	AInputPort();

	virtual void OnFacilityLinkedToPowerProvider() override;

	virtual void CompleteConstruction_Implementation() override;

	virtual void OnConnectedToFacility() override;

	////////////////////////////////////////////////////
	// IItemReceiver Functions
	////////////////////////////////////////////////////
	virtual bool CanReceiveItem() const override;

	virtual void OnConnectedToItemSender(AActor* NewSender) override;

	virtual void OnDisconnectedFromItemSender() override;

	virtual void OnReceiveItem(AActor* Item) override;

	virtual FVector GetReceivingDirection() const override;

protected:
	virtual void BeginPlay() override;

public:
	bool TryReceiveItem();

	void AutoConnectItemSender();

	AActor* GetPendingItemFromSender() const;

protected:

	UPROPERTY(VisibleAnywhere)
	UItemReceiveComponent* ItemReceiver;
};
