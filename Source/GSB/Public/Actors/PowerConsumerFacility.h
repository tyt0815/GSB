// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/ConstructibleFacility.h"
#include "PowerConsumerFacility.generated.h"

class IPowerProviderFacility;

UCLASS()
class GSB_API APowerConsumerFacility : public AConstructibleFacility
{
	GENERATED_BODY()

protected:
	// 주변 PowerProvider에게 전력을 받기 위해 연결을 시도한다.
	virtual void OnEnterWaitingState_Implementation() override;
	
	///////////////////////////////////////////////////////////
	// Link to PowerProvider
	///////////////////////////////////////////////////////////
public:
	bool IsLinkedToPowerProvider();
	virtual bool TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider);
	bool TryLinkToNearByPowerProvider();
	virtual void UnlinkFromPowerProvider();

	// LinkedPowerProvider와 LinkedPowerProviderInterface 변수를 설정한다.
	// 이 함수는 IPowerProviderFacility::TryLinkPowerConsumerFacility함수에서 호출되도록 설계되었으니 단독으로 호출 금지.
	void SetLinkedPowerProviderAndInterface(AFacility* PowerProvider, IPowerProviderFacility* PowerProviderInterface);
	void SetLinkedPowerProviderAndInterface(AFacility* PowerProvider);
	void SetLinkedPowerProviderAndInterface(IPowerProviderFacility* PowerProviderInterface);

protected:
	void TraceMultiPowerInfluenceArea(TArray<FHitResult>& HitResults);

	UPROPERTY(VisibleAnywhere)
	AFacility* LinkedPowerProvider;
	IPowerProviderFacility* LinkedPowerProviderInterface;


private:
	UPROPERTY(EditAnywhere, Category = "APowerConsumerFacility|Link to PowerProvider")
	int32 PowerConsumption = 10;

public:
	FORCEINLINE AFacility* GetLinkedPowerProvider() const
	{
		return LinkedPowerProvider;
	}
	FORCEINLINE int32 GetPowerConsumption() const
	{
		return PowerConsumption;
	}
};
