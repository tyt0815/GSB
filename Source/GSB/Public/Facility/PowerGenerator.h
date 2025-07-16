// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/HubLinkedFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "PowerGenerator.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API APowerGenerator : public AHubLinkedFacility,
	public IInputPortHandler
{
	GENERATED_BODY()
public:
	virtual void OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor) override;

	virtual void OnUnlinkFromPowerProvider_Implementation() override;

	virtual void RegisterInputPort(AInputPort* InInputPort) override;

public:


protected:
	UPROPERTY(EditAnywhere, Category = "GSB")
	class UItemDataAsset* ConsumingItemData;

private	:
	UFUNCTION()
	bool CanReceiveItem(const class AInputPort* InputPort);

	UFUNCTION()
	void OnReceiveItem(AActor* Item, class AInputPort* InputPort);

	bool TryBeginGeneraingPower();

	UFUNCTION()
	void EndGeneratingPower();

	bool IsGeneratingPower() const;

	AInputPort* ConnectedInputPort;

	UPROPERTY(EditAnywhere, Category = "GSB")
	float GeneratingTime = 10;

	FTimerHandle GeneratingTimerHandle;

	UPROPERTY(EditAnywhere, Category = "GSB")
	int32 MaxStack = 50;	

	int32 StoredItemStack = 0;
};
