// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Addon/ItemPort.h"
#include "Interfaces/ItemReceiver.h"
#include "Interfaces/ItemSender.h"
#include "OutputPort.generated.h"

class UItemSendComponent;
class UItemDataAsset;

UCLASS()
class GSB_API AOutputPort : public AItemPort, public IItemSender
{
	GENERATED_BODY()
	

public:
	AOutputPort();	

	virtual void OnFacilityLinkedToPowerProvider() override;

	virtual void CompleteConstruction_Implementation() override;

	virtual void OnConnectedToFacility() override;

	virtual bool TryConnectItemReceiver(AActor* NewReceiver) override;

	virtual void DisconnectItemReceiver() override;

	virtual AActor* GetItemToSend() const override;

protected:
	virtual void BeginPlay() override;

private:
	virtual bool TrySendItem() override;

public:
	TScriptInterface<IItemReceiver> GetConnectedItemReceiver() const;

	bool IsConnectedToReceiver();

	void AutoConnectItemReceiver();

	bool TryCreateAndSendItemCrate(UItemDataAsset* ItemData);

protected:
	UPROPERTY(VisibleAnywhere)
	UItemSendComponent* ItemSender;
};
