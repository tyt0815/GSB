// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBFacilityDetailWindow.generated.h"

class AFacility;

UCLASS()
class GSB_API UGSBFacilityDetailWindow : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void OnClosed() override;

public:
	void SetFacilityName(const FText InFacilityName);

	void UnlinkFacility();

	virtual void OnLinkedToFacility(AFacility* Facility);

	virtual void OnUnlinkedFromFacility();

protected:
	AFacility* LinkedFacility;

private:
	UPROPERTY(meta = (BindWidget))
	class UGSBFacilityName* FacilityName;
};
