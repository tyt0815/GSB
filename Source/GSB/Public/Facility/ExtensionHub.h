// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/PowerRelayFacility.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "ExtensionHub.generated.h"

class URetryPrioritizedActorRequestHandlerComponent;

UCLASS()
class GSB_API AExtensionHub : public APowerRelayFacility,  
	public IHubFacility,
	public IInputPortHandler,
	public IOutputPortHandler
{
	GENERATED_BODY()
	
public:
	AExtensionHub();

	virtual bool CanLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer);

	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;

	virtual UItemStorageComponent* GetHubStorageComponent() override;

	virtual void RegisterInputPort(AInputPort* InputPort) override;

	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* InputPortHandler;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* OutputPortHandler;

private:
	UFUNCTION()
	bool CanReceiveItem(const AInputPort* InputPort);

	UFUNCTION()
	bool TryReceiveItemFromInputPort(AActor* InputPort);

	UFUNCTION()
	bool TrySendItemToOutputPort(AActor* OutputPort);
};
