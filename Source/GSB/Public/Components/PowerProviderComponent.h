// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GridSizeBoxComponent.h"
#include "PowerProviderComponent.generated.h"

class APowerConsumerFacility;

UCLASS()
class GSB_API UPowerProviderComponent : public UGridSizeBoxComponent
{
	GENERATED_BODY()

public:
	UPowerProviderComponent();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	virtual void BeginPlay() override;
	
public:
	bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	void LinkFacilitiesInPowerInfluenceArea();

	void UnlinkAllPowerConsumerFacility();

	bool IsLinkedPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	void SetPowerInfluenceAreaVisibility(bool bVisibility, bool bPropagateToChildren = false);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<APowerConsumerFacility*> LinkedPowerConsumerFacilities;

	// 직접적으로 값을 변경하는것은 금지. UpdatePowerUsage를 통해 값을 변경한다.
	UPROPERTY(VisibleAnywhere)
	int32 CurrentPowerUsage = 0;

	UStaticMeshComponent* PowerInfluenceAreaMeshComponent;

public:
	FORCEINLINE int32 GetCurrentPowerUsage() const
	{
		return CurrentPowerUsage;
	}
	FORCEINLINE virtual void UpdatePowerUsage(int32 Addition)
	{
		CurrentPowerUsage += Addition;
	}
	FORCEINLINE	void SetPowerInfluenceAreaMeshComponent(UStaticMeshComponent* StaticMeshComponent)
	{
		PowerInfluenceAreaMeshComponent = StaticMeshComponent;
	}
};
