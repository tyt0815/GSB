// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/HubLinkedFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "PowerRelayFacility.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API APowerRelayFacility : public AHubLinkedFacility, public IPowerProviderFacility
{
	GENERATED_BODY()
	
public:
	APowerRelayFacility();

	virtual void BeginConstruction_Implementation() override;

	virtual void OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor) override;

	virtual void OnUnlinkFromPowerProvider_Implementation() override;

	virtual void PostUnlinkFromPowerProvider_Implementation() override;

	virtual int32 GetTotalPowerUsage() const override;

	/////////////////////////////////////////////////////
	// IPowerProviderFacility Functions
	/////////////////////////////////////////////////////

	virtual void UpdatePowerUsage(int32 Addition) override;

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override final;

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer) override;

	virtual bool CanProvidePower() override;

	virtual void SetPowerInfluenceAreaVisibility(bool bVisibilty) override;


protected:
	virtual void BeginPlay() override;

	virtual void PreTurnOn_Implementation() override;

	virtual void PostTurnOn_Implementation() override;

	virtual void PreTurnOff_Implementation() override;

	virtual void PostTurnOff_Implementation() override;

	virtual bool CanLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer);

protected:
	UPROPERTY(VisibleAnywhere)
	UPowerProviderComponent* PowerProviderComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PowerInfluenceAreaStaticMeshComponent;
};
