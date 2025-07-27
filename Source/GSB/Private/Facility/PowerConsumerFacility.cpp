// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/PowerConsumerFacility.h"
#include "Facility/Wire.h"
#include "Facility/Addon/FacilityAddon.h"
#include "HUDs/GSBPoweredFacilityDetailWindow.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
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
	if (UGSBPoweredFacilityDetailWindow* Window = Cast<UGSBPoweredFacilityDetailWindow>(DetailWindow))
	{
		UpdatePowerWidgets();
		Window->OnPowerSwitchClicked.AddDynamic(this, &APowerConsumerFacility::HandleOnPowerSwitchClicked);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("UGSBPoweredFacilityDetailWindow캐스팅 실패"));
	}
}

bool APowerConsumerFacility::IsLinkedToPowerProvider() const
{
	return IsValidPowerProviderScriptInterface(LinkedPowerProvider);
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
	return PowerProvider != nullptr && IsOn() && PowerProvider->CanProvidePower();
}

void APowerConsumerFacility::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	// PowerWire 연결
	if (AFacility* PowerProviderFacility = Cast<AFacility>(PowerProviderActor))
	{
		if (UClass* PowerWireClass = UGSBGameInstance::GetActorClass(this, (TEXT("PowerWire"))))
		{
			if (UWorld* World = GetWorld())
			{
				if (!IsValid(PowerWire))
				{
					PowerWire = World->SpawnActor<AWire>(PowerWireClass);
				}
				if (PowerWire)
				{
					FVector Offset = FVector(0, 0, -2);
					FVector Start = GetLocationOnTopXYPlane() + Offset;
					FVector PowerProviderTop = PowerProviderFacility->GetLocationOnTopXYPlane();
					FVector WireForward = PowerProviderTop - Start;
					WireForward.Normalize();
					FVector	WireRight = FVector::ZAxisVector.Cross(WireForward);
					WireRight.Normalize();
					FVector WireUp = WireForward.Cross(WireRight);

					FVector TraceEnd = (Start + PowerProviderTop) / 2;
					FVector TraceStart = TraceEnd + WireUp * 500;

					TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
					ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
					TArray<AActor*> ActorsToIgnore;
					TArray<FHitResult> OutHits;
					UKismetSystemLibrary::BoxTraceMultiForObjects(
						this,
						TraceStart,
						TraceEnd,
						FVector(0, 0, (PowerProviderTop - Start).Length() / 2),
						(-WireUp).Rotation(),
						ObjectTypes,
						false,
						ActorsToIgnore,
						EDrawDebugTrace::None,
						OutHits,
						true
					);

					float ZoHitResult1 = -1;
					float ZoHitResult2 = -1;
					FVector ImpactPoint1 = PowerProviderTop;
					FVector ImpactPoint2 = PowerProviderTop;
					for (const FHitResult& HitResult : OutHits)
					{
						if (!HitResult.GetActor()->ActorHasTag(TEXT("Antenna")) || HitResult.GetActor() != PowerProviderActor)
						{
							continue;
						}
						FVector UnitVector = HitResult.ImpactPoint - Start;
						UnitVector.Normalize();
						float ZoHitResult = FVector::ZAxisVector.Dot(UnitVector);
						if (ZoHitResult > ZoHitResult1)
						{
							ZoHitResult2 = ZoHitResult1;
							ImpactPoint2 = ImpactPoint1;
							ZoHitResult1 = ZoHitResult;
							ImpactPoint1 = HitResult.ImpactPoint;
						}
						else if (ZoHitResult > ZoHitResult2)
						{
							ZoHitResult2 = ZoHitResult;
							ImpactPoint2 = HitResult.ImpactPoint;
						}
					}
					FVector End = (ImpactPoint1 + ImpactPoint2) / 2;

					PowerWire->Link(Start, End);
				}
			}
		}
	}
	

	LinkedPowerProvider = TScriptInterface<IPowerProviderFacility>(PowerProviderActor);
	if (LinkedPowerProvider)
	{
		LinkedPowerProvider->UpdatePowerUsage(PowerConsumption);
	}
	for (AFacilityAddon* Addon : ConnectedAddons)
	{
		Addon->OnFacilityLinkedToPowerProvider();
	}
	UpdatePowerWidgets();
}

void APowerConsumerFacility::OnUnlinkFromPowerProvider_Implementation()
{
	if (IsValid(PowerWire))
	{
		PowerWire->Unlink();
	}
	PowerWire = nullptr;

	TArray<IPowerProviderFacility*> PowerProviderToIgnore;
	if (LinkedPowerProvider)
	{
		PowerProviderToIgnore.Add(LinkedPowerProvider.GetInterface());
		LinkedPowerProvider->UpdatePowerUsage(-PowerConsumption);
	}
	LinkedPowerProvider = nullptr;

	if (IsOn())
	{
		TRACE_SCREEN_LOG(FString::Printf(TEXT("%s: Sibal"), *GetFacilityName().ToString()));
		TryLinkToNearByPowerProvider(PowerProviderToIgnore);
	}

	UpdatePowerWidgets();
}

bool APowerConsumerFacility::TryLinkToNearByPowerProvider(const TArray<IPowerProviderFacility*> PowerProvidersToIgnore)
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
		if (PowerProvidersToIgnore.Contains(PowerProvider))
		{
			continue;
		}
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

bool APowerConsumerFacility::TryTurnOn()
{
	if (IsConstructed())
	{
		TurnOn();
		return true;
	}
	return false;
}

void APowerConsumerFacility::TurnOff()
{
	bOn = false;
	UnlinkFromPowerProvider();
	UpdatePowerWidgets();
}

int32 APowerConsumerFacility::GetTotalPowerUsage() const
{
	return PowerConsumption;
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

void APowerConsumerFacility::UpdatePowerWidgets()
{
	UpdatePowerStatusWidget();
	UpdatePowerConsumptionWidget();
}

void APowerConsumerFacility::UpdatePowerStatusWidget()
{
	if (IsValid(DetailWindow))
	{
		if (UGSBPoweredFacilityDetailWindow* Window = Cast<UGSBPoweredFacilityDetailWindow>(DetailWindow))
		{
			if (!IsOn())
			{
				Window->SetPowerStatus_TurnedOff();
			}
			else if (IsOperating())
			{
				Window->SetPowerStatus_Powered();
			}
			else
			{
				Window->SetPowerStatus_Unpowered();
			}
		}
	}	
}

void APowerConsumerFacility::UpdatePowerConsumptionWidget()
{
	if (IsValid(DetailWindow))
	{
		if (UGSBPoweredFacilityDetailWindow* Window = Cast<UGSBPoweredFacilityDetailWindow>(DetailWindow))
		{
			Window->SetPowerConsumption(GetTotalPowerUsage());
		}
	}
}

void APowerConsumerFacility::TurnOn()
{
	bOn = true;
	TryLinkToNearByPowerProvider({});
	UpdatePowerWidgets();
}

void APowerConsumerFacility::HandleOnPowerSwitchClicked()
{
	if (bOn)
	{
		TurnOff();
	}
	else
	{
		TryTurnOn();
	}
}