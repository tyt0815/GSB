// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Addon/FacilityAddon.h"
#include "ItemPort.generated.h"

class USplineTransportComponent;

UCLASS()
class GSB_API AItemPort : public AFacilityAddon
{
	GENERATED_BODY()
	
public:	
	AItemPort();

protected:
	virtual void BeginPlay() override;

protected:
	bool bHasToken = false;

public:
	FORCEINLINE bool HasToken() const
	{
		return bHasToken;
	}
	FORCEINLINE void SetHasToken(bool bHas)
	{
		bHasToken = bHas;
	}
};
