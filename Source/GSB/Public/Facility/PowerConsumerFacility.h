// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "PowerConsumerFacility.generated.h"

class UGSBFacilityPowerSwitch;


UCLASS()
class GSB_API APowerConsumerFacility : public AConstructibleFacility
{
	GENERATED_BODY()
public:

	virtual bool IsOperating() const override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void BeginConstruction_Implementation() override;

	virtual void CompleteConstruction_Implementation() override;

	virtual void OnShowDetailInteraction(AActor* Interactor) override;

public:
	bool IsLinkedToPowerProvider() const;

	bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void OnLinkToPowerProvider(AActor* PowerProviderActor);
	virtual void OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor);

	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void OnUnlinkFromPowerProvider();
	virtual void OnUnlinkFromPowerProvider_Implementation();

	bool TryLinkToNearByPowerProvider();

	void UnlinkFromPowerProvider();

	bool TryTurnOn();
	
	virtual void TurnOff();

	virtual int32 GetTotalPowerUsage() const;

protected:
	void TraceMultiPowerInfluenceArea(TArray<FHitResult>& HitResults);

	bool IsValidPowerProviderScriptInterface(const TScriptInterface<IPowerProviderFacility>& PowerProvider) const;

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IPowerProviderFacility> LinkedPowerProvider = nullptr;

	void UpdatePowerWidgets();

	void UpdatePowerStatusWidget();

	void UpdatePowerConsumptionWidget();

	virtual void TurnOn();

private:
	class AWire* PowerWire;

	UFUNCTION()
	void HandleOnPowerSwitchClicked();

	UPROPERTY(EditAnywhere, Category = "APowerConsumerFacility|Link to PowerProvider")
	int32 PowerConsumption = 10;

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
