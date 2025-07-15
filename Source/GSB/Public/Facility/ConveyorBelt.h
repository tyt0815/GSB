// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/ItemReceiver.h"
#include "Interfaces/ItemSender.h"
#include "ConveyorBelt.generated.h"

class UItemReceiveComponent;
class UItemSendComponent;
class USplineTransportComponent;

UCLASS()
class GSB_API AConveyorBelt : public AConstructibleFacility, public IItemReceiver, public IItemSender
{
	GENERATED_BODY()
	
public:
	AConveyorBelt();

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

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

protected:
	virtual void BeginPlay() override;

public:
	void DeconstructConnectedConveyorChain();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineTransportComponent* TransportComponent;

	UPROPERTY(VisibleAnywhere)
	UItemReceiveComponent* ItemReceiveComponent;

	UPROPERTY(VisibleAnywhere)
	UItemSendComponent* ItemSendComponent;

	UPROPERTY(EditAnywhere)
	AActor* DebugPayload;

	void DeconstructConnectedReceiverConveyorChain(bool bDeconstructSelf);

	void DeconstructConnectedSenderConveyorChain(bool bDeconstructSelf);

private:
	bool TryAutoConnectToItemSender();
	bool TryAutoConnectItemReceiver();

	UFUNCTION()		
	void SetItemToSend(AActor* Item);
};
