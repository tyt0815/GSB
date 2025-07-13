// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBStorage.h"
#include "GSBInventory.generated.h"



UCLASS()
class GSB_API UGSBInventory : public UGSBStorage
{
	GENERATED_BODY()
public:
	void SetWeight(int32 CurrentWeight, int32 MaxWeight);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Weight;
};
