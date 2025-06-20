// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PowerConsumerFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

void APowerConsumerFacility::OnEnterWaitingState_Implementation()
{
	Super::OnEnterWaitingState_Implementation();
	TryLinkToNearByPowerProvider();
}

bool APowerConsumerFacility::IsLinkedToPowerProvider()
{
	return IsValid(LinkedPowerProvider) && LinkedPowerProviderInterface;
}

bool APowerConsumerFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	AFacility* Facility = Cast<AFacility>(PowerProvider);
	if (IsValid(Facility) && PowerProvider)
	{
		return PowerProvider->TryLinkPowerConsumerFacility(this);
	}
	return false;
}

bool APowerConsumerFacility::TryLinkToNearByPowerProvider()
{
	if (IsLinkedToPowerProvider())
	{
		return true;
	}

	TArray<FHitResult> HitResults;
	TraceMultiPowerInfluenceArea(HitResults);

	for (const FHitResult& HitResult : HitResults)
	{
		IPowerProviderFacility* PowerProvider = Cast<IPowerProviderFacility>(HitResult.GetActor());
		if (TryLinkToPowerProvider(PowerProvider))
		{
			return true;
		}
	}

	return false;
}

void APowerConsumerFacility::UnlinkFromPowerProvider()
{
	if (IsLinkedToPowerProvider())
	{
		LinkedPowerProviderInterface->UnlinkPowerConsumerFacility(this);
		IPowerProviderFacility* ThisInterface = Cast<IPowerProviderFacility>(this);
		if (ThisInterface)
		{
			ThisInterface->UpdatePowerUsage(-PowerConsumption);
		}
	}
}

void APowerConsumerFacility::SetLinkedPowerProviderAndInterface(AFacility* PowerProvider, IPowerProviderFacility* PowerProviderInterface)
{
	LinkedPowerProvider = PowerProvider;
	LinkedPowerProviderInterface = PowerProviderInterface;
}

void APowerConsumerFacility::SetLinkedPowerProviderAndInterface(AFacility* PowerProvider)
{
	IPowerProviderFacility* PowerProviderInterface = Cast<IPowerProviderFacility>(PowerProvider);
	SetLinkedPowerProviderAndInterface(PowerProvider, PowerProviderInterface);
}

void APowerConsumerFacility::SetLinkedPowerProviderAndInterface(IPowerProviderFacility* PowerProviderInterface)
{
	AFacility* PowerProvider = Cast<AFacility>(PowerProviderInterface);
	SetLinkedPowerProviderAndInterface(PowerProvider, PowerProviderInterface);
}

void APowerConsumerFacility::TraceMultiPowerInfluenceArea(TArray<FHitResult>& HitResults)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	BoxTraceMultiFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResults);
	TArray<int32> IndicesToRemove;
	for (int32 i = 0; i < HitResults.Num(); ++i)
	{
		// Preview or Constructing State인 시설은 제외한다.
		AConstructibleFacility* Facility = Cast<AConstructibleFacility>(HitResults[i].GetActor());
		if (Facility &&
			EFacilityState::EFS_None !=
			(Facility->GetFacilityState() & (EFacilityState::EFS_Preview | EFacilityState::EFS_Constructing))
			)
		{
			IndicesToRemove.Add(i);
		}
	}

	for (int i = IndicesToRemove.Num() - 1; i >= 0; --i)
	{
		HitResults.RemoveAt(IndicesToRemove[i]);
	}
}
