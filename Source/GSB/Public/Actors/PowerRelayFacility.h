// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HubLinkedFacility.h"
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

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual bool CanProvidePower() override;

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility) override;

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumer) override;

	// 주변 PowerConsumer를 자신에게 연결해 전력 공급을 시도한다.
	virtual void OnEnterWaitingState_Implementation() override;

	// PowerProvider에게 전력을 공급받고 있을 경우, 호환되는 주변 PowerConsumer를 자신에게 연결해 전력 공급을 시도한다.
	virtual void LinkFacilitiesInPowerInfluenceArea() override;

	// Link에 성공하면 주변 Link되지 않은 PowerConsumer들의 Link를 시도한다.
	virtual bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider) override;

	// PowerRelay에 링크되어 전력을 받는 시설의 링크를 재귀적으로 모두 끊는다.
	virtual void UnlinkFromPowerProvider() override;

	virtual void UpdatePowerUsage(int32 Addition) override final;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PowerInfluenceAreaComponent;

	UPROPERTY(EditAnywhere, Category = "AExtensionHub")
	uint32 PowerInfluenceAreaRadiusInGridUnits;

public:
	FORCEINLINE virtual UStaticMeshComponent* GetPowerInfluenceAreaComponent() const override final
	{
		return PowerInfluenceAreaComponent;
	}
	FORCEINLINE virtual uint32 GetPowerInfluenceAreaRadiusInGridUnits() const override final
	{
		return PowerInfluenceAreaRadiusInGridUnits;
	}
};
