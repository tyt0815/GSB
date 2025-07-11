// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerConsumerFacility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "HUDs/GSBFacilityPowerStatus.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Kismet/KismetStringLibrary.h"
#include "DebugHeader.h"

bool APowerConsumerFacility::IsOperating() const
{
	return Super::IsOperating() && IsOn() && IsLinkedToPowerProvider() && LinkedPowerProvider->CanProvidePower();
}

void APowerConsumerFacility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnlinkFromPowerProvider();

	Super::EndPlay(EndPlayReason);
}

void APowerConsumerFacility::BeginConstruction_Implementation()
{
	Super::BeginConstruction_Implementation();
	TurnOff();
}

void APowerConsumerFacility::CompleteConstruction_Implementation()
{
	Super::CompleteConstruction_Implementation();
	TurnOn();
}

void APowerConsumerFacility::OnShowDetailInteraction(AActor* Interactor)
{
	Super::OnShowDetailInteraction(Interactor);
	if (PowerStatusClass)
	{
		PowerStatus = Cast<UGSBFacilityPowerStatus>(AttachDetailWindowHead(PowerStatusClass));
		if (!PowerStatus)
		{
			TRACE_SCREEN_LOG(TEXT("UGSBFacilityPowerStatus 캐스팅 실패"));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("PowerStatusClass가 nullptr 입니다."));
	}
}

bool APowerConsumerFacility::IsLinkedToPowerProvider() const
{
	return IsOn() && IsValidPowerProviderScriptInterface(LinkedPowerProvider);
}

bool APowerConsumerFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	if (CanLinkToPowerProvider(PowerProvider))
	{
		return PowerProvider->TryLinkPowerConsumerFacility(this);
	}
	return false;
}

bool APowerConsumerFacility::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	return PowerProvider != nullptr;
}

void APowerConsumerFacility::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	UnlinkFromPowerProvider();
	LinkedPowerProvider = TScriptInterface<IPowerProviderFacility>(PowerProviderActor);
	if (LinkedPowerProvider)
	{
		LinkedPowerProvider->UpdatePowerUsage(PowerConsumption);
	}
	for (AFacilityAddon* Addon : ConnectedAddons)
	{
		Addon->OnFacilityLinkedToPowerProvider();
	}
}

void APowerConsumerFacility::OnUnlinkFromPowerProvider_Implementation()
{
	if (LinkedPowerProvider)
	{
		LinkedPowerProvider->UpdatePowerUsage(-PowerConsumption);
	}
	LinkedPowerProvider = nullptr;
}

bool APowerConsumerFacility::TryLinkToNearByPowerProvider()
{
	if (IsLinkedToPowerProvider())
	{
		return false;
	}

	if (!IsOn())
	{
		return false;
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
		LinkedPowerProvider->UnlinkPowerConsumerFacility(this);
	}
}

bool APowerConsumerFacility::TryTurnOn_Implementation()
{
	if (IsConstructed())
	{
		TurnOn();
		return true;
	}
	return false;
}

void APowerConsumerFacility::TurnOff_Implementation()
{
	bOn = false;
	UnlinkFromPowerProvider();
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
		if (Facility && !Facility->IsConstructed())
		{
			IndicesToRemove.Add(i);
		}
	}

	for (int i = IndicesToRemove.Num() - 1; i >= 0; --i)
	{
		HitResults.RemoveAt(IndicesToRemove[i]);
	}
}

bool APowerConsumerFacility::IsValidPowerProviderScriptInterface(const TScriptInterface<IPowerProviderFacility>& PowerProvider) const
{
	return IsValid(PowerProvider.GetObject()) && PowerProvider.GetInterface();
}

void APowerConsumerFacility::TurnOn()
{
	bOn = true;
	TryLinkToNearByPowerProvider();
}