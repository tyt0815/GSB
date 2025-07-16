// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Addon/ItemPort.h"
#include "Interfaces/ItemReceiver.h"
#include "Interfaces/ItemSender.h"
#include "InputPort.generated.h"

class UItemReceiveComponent;

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FCanReceiveItemSignature, const class AInputPort*, InputPort);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputPortRecievedItemSignature, AActor*, Item, class AInputPort*, InputPort);

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

	FCanReceiveItemSignature OnCanReceiveItemCalled;

protected:
	virtual void BeginPlay() override;

public:
	bool TryReceiveItem();

	void AutoConnectItemSender();

	AActor* GetPendingItemFromSender() const;

	AActor* TakeReceivedItem();

	TScriptInterface<IItemSender> GetConnectedItemSender() const;

	void DeconstructConnectedConveyorBeltChain();

	FOnInputPortRecievedItemSignature OnReceivedItemDelegate;

protected:

	UPROPERTY(VisibleAnywhere)
	UItemReceiveComponent* ItemReceiver;
};
