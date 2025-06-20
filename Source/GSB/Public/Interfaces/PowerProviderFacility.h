// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PowerProviderFacility.generated.h"

constexpr float DEFAULT_CYLINDER_RADIUS = 50.0f;
constexpr float DEFAULT_CYLINDER_HALF_HEIGHT = 50.0f;

class APowerConsumerFacility;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPowerProviderFacility : public UInterface
{
	GENERATED_BODY()
};


class GSB_API IPowerProviderFacility
{
	GENERATED_BODY()

public:
	// Height는 GridSize.Z값을 입력한다.
	void UpdatePowerInfluenceAreaScale(uint32 HeightInGridUnits);

	// Cylinder 형태의 범위를 구현하고 싶지만 Collision 컴포넌트중 실린더는 없음. 따라서 실린더 형태의 메시를 사용한다.
	virtual UStaticMeshComponent* GetPowerInfluenceAreaComponent() const = 0;


	// 실린더 형태의 파워 영향권의 반지름을 설정하는데 사용한다. 실제 길이와 1:1 매칭이 되는 것이 아니라 AFacility의
	// GridSize가 GridBounds(BoxComponent)의 크기를 설정하는 것과 같이 설정되도록 한다. 즉 GridSize.X값과 이 변수의
	// 값이 일치하면, GridBounds의 BoxExtent.X의 길이와 실제 실린더의 반지름이 일치해야 한다.
	virtual uint32 GetPowerInfluenceAreaRadiusInGridUnits() const = 0;

	virtual bool CanProvidePower() = 0;

	virtual bool TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	virtual void UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	virtual void LinkFacilitiesInPowerInfluenceArea();

	void UnlinkAllPowerConsumerFacility();

	bool IsLinkedPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility);

	void GetFacilitiesInPowerInfluenceArea(TArray<AActor*>& Facilities);

protected:
	TArray<APowerConsumerFacility*> LinkedPowerConsumerFacilities;

private:
	// 직접적으로 값을 변경하는것은 금지. UpdatePowerUsage를 통해 값을 변경한다.
	int32 CurrentPowerUsage = 0;

public:
	FORCEINLINE int32 GetCurrentPowerUsage() const
	{
		return CurrentPowerUsage;
	}
	FORCEINLINE virtual void UpdatePowerUsage(int32 Addition)
	{
		CurrentPowerUsage += Addition;
	}
	FORCEINLINE void SetPowerInfluenceAreaVisibility(bool bVisibility, bool bPropagateToChildren = false)
	{
		GetPowerInfluenceAreaComponent()->SetVisibility(bVisibility, bPropagateToChildren);
	}
};
