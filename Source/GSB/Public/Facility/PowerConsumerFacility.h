// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/ConstructibleFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "PowerConsumerFacility.generated.h"


UCLASS()
class GSB_API APowerConsumerFacility : public AConstructibleFacility
{
	GENERATED_BODY()

protected:
	virtual void BeginConstruction_Implementation() override;

	virtual void CompleteConstruction_Implementation() override;

public:
	bool IsLinkedToPowerProvider();

	bool IsActivate();

	bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	virtual bool CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider);

	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void OnLinkToPowerProvider();
	virtual void OnLinkToPowerProvider_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void OnUnlinkFromPowerProvider();
	virtual void OnUnlinkFromPowerProvider_Implementation();
	bool TryLinkToNearByPowerProvider();

	void UnlinkFromPowerProvider();

	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void TurnOn();
	virtual void TurnOn_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, Category = "APowerConsumerFacility|LinkToPowerProvider")
	void TurnOff();
	virtual void TurnOff_Implementation();

protected:
	void TraceMultiPowerInfluenceArea(TArray<FHitResult>& HitResults);

	bool IsValidPowerProviderScriptInterface(TScriptInterface<IPowerProviderFacility> PowerProvider);

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IPowerProviderFacility> LinkedPowerProvider = nullptr;

private:

	UPROPERTY(EditAnywhere, Category = "APowerConsumerFacility|Link to PowerProvider")
	int32 PowerConsumption = 10;

	bool bOn = true;
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
		return IsConstructed() && bOn;
	}

	FORCEINLINE void SetLinkedPowerProvider(TScriptInterface<IPowerProviderFacility> NewPowerProvider)
	{
		LinkedPowerProvider = NewPowerProvider;
	}
};
