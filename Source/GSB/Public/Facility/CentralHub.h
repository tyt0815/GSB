// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Facility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Interfaces/HubFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "HUDs/GSBWindowHead.h"
#include "CentralHub.generated.h"

class URetryPrioritizedActorRequestHandlerComponent;
class UItemStorageComponent;

UCLASS()
class GSB_API ACentralHub : 
	public AFacility,
	public IPowerProviderFacility,
	public IHubFacility,
	public IInputPortHandler,
	public IOutputPortHandler
{
	GENERATED_BODY()
	
public:
	ACentralHub();

	virtual void Tick(float Delta) override;

	virtual void OnShowDetailInteraction(AActor* Interactor) override;

	virtual bool CanProvidePower() override;

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual void UpdatePowerUsage(int32 Addition) override;

	virtual void SetPowerInfluenceAreaVisibility(bool bVisibilty) override;

	virtual void RegisterInputPort(AInputPort* InputPort) override;

	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;

	virtual UItemStorageComponent* GetHubStorageComponent() override;

protected:
	virtual void BeginPlay() override;

protected:
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

	UPROPERTY(EditDefaultsOnly, Category = "ACentralHub|UI")
	TSubclassOf<UGSBWindowHead> PowerStatusClass;

	UFUNCTION()
	bool TryReceiveItemFromInputPort(AActor* InputPort);

	UFUNCTION()
	bool TrySendItemToOutputPort(AActor* OutputPort);
	
	UFUNCTION()
	bool CanReceiveItem(const AInputPort* InputPort);

private:
	UPROPERTY(EditAnywhere, Category = "ACentralHub|Power")
	int PowerCapacity = 500;

	class UGSBFacilityPowerStatus* PowerStatus;

	class AGSBHUD* GSBHUD;

	class UGSBPlayerOverlay* Overlay;

	void UpdatePowerStatusWidget();

	UFUNCTION()
	void SetOverlayWidget();
};
