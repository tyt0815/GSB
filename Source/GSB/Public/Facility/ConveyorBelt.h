// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/ItemReceiver.h"
#include "Interfaces/ItemSender.h"
#include "Interfaces/ChainDeconstrutableFacility.h"
#include "ConveyorBelt.generated.h"

class UItemReceiveComponent;
class UItemSendComponent;
class USplineTransportComponent;

UCLASS()
class GSB_API AConveyorBelt : 
	public AConstructibleFacility,
	public IItemReceiver,
	public IItemSender,
	public IChainDeconstrutableFacility
{
	GENERATED_BODY()
	
public:
	AConveyorBelt();

	virtual void Tick_OnOperating(float DeltaTime) override;

	virtual void CompleteConstruction_Implementation() override;

	////////////////////////////////////////////////////////////
	// IItemReceiver Functions
	////////////////////////////////////////////////////////////
	virtual bool CanReceiveItem() const override;

	virtual void OnConnectedToItemSender(AActor* NewSender) override;

	virtual void OnDisconnectedFromItemSender() override;

	virtual void OnReceiveItem(AActor* Item) override;

	virtual FVector GetReceivingDirection() const override;
	////////////////////////////////////////////////////////////
	// IItemSender Functions
	////////////////////////////////////////////////////////////
	virtual bool TrySendItem() override;

	virtual bool TryConnectItemReceiver(AActor* NewReceiver) override;

	virtual void DisconnectItemReceiver() override;

	virtual AActor* GetItemToSend() const override;
	////////////////////////////////////////////////////////////
	// IChainDeconstrutableFacility Functions
	////////////////////////////////////////////////////////////
	virtual void DeconstructConnectedConveyorChain() override;

	virtual void DeconstructConnectedReceiverConveyorChain(bool bDeconstructSelf) override;

	virtual void DeconstructConnectedSenderConveyorChain(bool bDeconstructSelf) override;

protected:
	virtual void BeginPlay() override;

	virtual void BeginDeconstruction_Implementation() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineTransportComponent* TransportComponent;

	UPROPERTY(VisibleAnywhere)
	UItemReceiveComponent* ItemReceiveComponent;

	UPROPERTY(VisibleAnywhere)
	UItemSendComponent* ItemSendComponent;

	UPROPERTY(EditAnywhere)
	AActor* DebugPayload;

private:
	bool TryAutoConnectToItemSender();
	bool TryAutoConnectItemReceiver();

	UFUNCTION()		
	void SetItemToSend(AActor* Item);
};
