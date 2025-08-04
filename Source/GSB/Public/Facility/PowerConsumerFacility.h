// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Interfaces/PowerWireConnection.h"
#include "PowerConsumerFacility.generated.h"

class UGSBFacilityPowerSwitch;


UCLASS()
class GSB_API APowerConsumerFacility : public AConstructibleFacility, public IPowerWireConnection
{
	GENERATED_BODY()
public:
	APowerConsumerFacility();

	virtual bool IsOperating() const override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void BeginConstruction_Implementation() override;

	virtual void CompleteConstruction_Implementation() override;

	virtual void OnShowDetailInteraction(AActor* Interactor) override;

	virtual FVector GetPowerWireConnectionPoint() const final;

public:
	bool IsLinkedToPowerProvider() const;

	bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	bool TryConnectPowerWire(AActor* Actor);

	void DisconnectPowerWire();

	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PreLinkToPowerProvider();
	virtual void PreLinkToPowerProvider_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void OnLinkToPowerProvider(AActor* PowerProviderActor);
	virtual void OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor);

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PostLinkToPowerProvider();
	virtual void PostLinkToPowerProvider_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PreUnlinkFromPowerProvider();
	virtual void PreUnlinkFromPowerProvider_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void OnUnlinkFromPowerProvider();
	virtual void OnUnlinkFromPowerProvider_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PostUnlinkFromPowerProvider();
	virtual void PostUnlinkFromPowerProvider_Implementation();

	bool TryLinkToNearByPowerProvider();

	void UnlinkFromPowerProvider();

	bool TryTurnOn();
	
	void TurnOff();

	virtual int32 GetTotalPowerUsage() const;

protected:
	void TraceMultiPowerInfluenceArea(TArray<FHitResult>& HitResults);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PowerWireConnectionBoundsComponent;

	UPROPERTY(VisibleAnywhere)
	AActor* LinkedPowerProvider = nullptr;
	IPowerProviderFacility* LinkedPowerProviderInterface = nullptr;

	void UpdatePowerWidgets();

	void UpdatePowerStatusWidget();

	void UpdatePowerConsumptionWidget();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PreTurnOn();
	virtual void PreTurnOn_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PostTurnOn();
	virtual void PostTurnOn_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PreTurnOff();
	virtual void PreTurnOff_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "GSB|PowerConsumerFacility")
	void PostTurnOff();
	virtual void PostTurnOff_Implementation();

	UPROPERTY(EditAnywhere, Category = "GSB|PowerConsumerFacility")
	int32 PowerConsumption = 0;

private:
	class AWire* PowerWire;

	UFUNCTION()
	void HandleOnPowerSwitchClicked();

	class UGSBFacilityPowerStatus* PowerStatusWidget;

	class UGSBPowerConsumption* PowerConsumptionWidget;

	bool bOn = false;
public:
	FORCEINLINE TScriptInterface<IPowerProviderFacility> GetLinkedPowerProvider() const
	{
		return LinkedPowerProvider;
	}
	FORCEINLINE int32 GetPowerConsumption() const
	{
		return PowerConsumption;
	}
	FORCEINLINE bool IsOn() const
	{
		return bOn;
	}
};
