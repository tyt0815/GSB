// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Facility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "Interfaces/PowerWireConnection.h"
#include "CentralHub.generated.h"

class ACentralHub;
class URetryPrioritizedActorRequestHandlerComponent;
class UItemStorageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePowerUsage, int32, PowerUsage);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatePowerCapacity, int32, PowerCapacity);

UCLASS()
class GSB_API ACentralHub : 
	public AFacility,
	public IPowerProviderFacility,
	public IHubFacility,
	public IInputPortHandler,
	public IOutputPortHandler,
	public IPowerWireConnection
{
	GENERATED_BODY()
	
public:
	ACentralHub();

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void PostInitializeComponents() override;

	virtual bool CanProvidePower() override;

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual void UpdatePowerUsage(int32 Addition) override;

	virtual void SetPowerInfluenceAreaVisibility(bool bVisibilty) override;

	virtual void RegisterInputPort(AInputPort* InputPort) override;

	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;

	virtual UItemStorageComponent* GetHubStorageComponent() override;

	virtual FVector GetPowerWireConnectionPoint() const override;

protected:

	virtual void BeginPlay() override;

public:
	void UpdatePowerCapacity(int32 Addition);

	FOnUpdatePowerUsage OnUpdatePowerUsage;

	FOnUpdatePowerCapacity OnUpdatePowerCapacity;

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PowerWireConnectionBoundsComponent;

	UPROPERTY(VisibleAnywhere)
	UPowerProviderComponent* PowerProviderComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PowerInfluenceAreaStaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UItemStorageComponent* StorageComponent;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* InputPortHandler;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* OutputPortHandler;

	UFUNCTION()
	bool TryReceiveItemFromInputPort(AActor* InputPort);

	UFUNCTION()
	bool TrySendItemToOutputPort(AActor* OutputPort);
	
	UFUNCTION()
	bool CanReceiveItem(const AInputPort* InputPort);

private:
	UPROPERTY(EditAnywhere, Category = "ACentralHub|Power")
	int PowerCapacity = 500;
};
