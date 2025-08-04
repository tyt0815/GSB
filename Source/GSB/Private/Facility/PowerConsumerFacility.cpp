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

APowerConsumerFacility::APowerConsumerFacility()
{
	PowerWireConnectionBoundsComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PowerWireConnectionBounds"));
	PowerWireConnectionBoundsComponent->SetupAttachment(GetRootComponent());
	PowerWireConnectionBoundsComponent->SetCollisionProfileName(TEXT("PowerWireConnectionBounds"));
	PowerWireConnectionBoundsComponent->Mobility = EComponentMobility::Static;
}

bool APowerConsumerFacility::IsOperating() const
{
	return Super::IsOperating() && IsOn() && IsLinkedToPowerProvider() && LinkedPowerProviderInterface->CanProvidePower();
}

void APowerConsumerFacility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TurnOff();

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
	check(IsConstructed());
	TryTurnOn();
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

FVector APowerConsumerFacility::GetPowerWireConnectionPoint() const
{
	return PowerWireConnectionBoundsComponent->GetComponentLocation();
}

bool APowerConsumerFacility::IsLinkedToPowerProvider() const
{
	return IsValid(LinkedPowerProvider);
}

bool APowerConsumerFacility::TryLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	if (CanLinkToPowerProvider(PowerProvider))
	{
		return PowerProvider->TryLinkPowerConsumerFacility(this);
	}
	return false;
}

bool APowerConsumerFacility::TryConnectPowerWire(AActor* Actor)
{
	if (IPowerWireConnection* PowerWireConnectionInterface = Cast<IPowerWireConnection>(Actor))
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
					FVector Start = GetPowerWireConnectionPoint();
					FVector PowerProviderTop = PowerWireConnectionInterface->GetPowerWireConnectionPoint();
					FVector WireForward = PowerProviderTop - Start;
					WireForward.Normalize();
					FVector	WireRight = FVector::ZAxisVector.Cross(WireForward);
					WireRight.Normalize();
					FVector WireUp = WireForward.Cross(WireRight);

					FVector TraceEnd = (Start + PowerProviderTop) / 2;
					FVector TraceStart = TraceEnd + WireUp * 500;

					TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
					ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel8));
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
						true,
						FLinearColor::Red,
						FLinearColor::Green,
						60
					);


					FVector End = PowerProviderTop;
					float ZoHitMax = -1;
					for (const FHitResult& HitResult : OutHits)
					{
						if (!IsValid(HitResult.GetActor()))
						{
							continue;
						}

						if (HitResult.GetActor() != Actor && HitResult.GetActor()->GetParentActor() != Actor)
						{
							continue;
						}

						FVector UnitVector = HitResult.ImpactPoint - Start;
						UnitVector.Normalize();
						float ZoHitResult = FVector::ZAxisVector.Dot(UnitVector);
						if (ZoHitResult > ZoHitMax)
						{
							ZoHitMax = ZoHitResult;
							End = HitResult.ImpactPoint;
						}
					}

					PowerWire->Connect(Start, End);
					return true;
				}
			}
		}
	}
	return false;
}

void APowerConsumerFacility::DisconnectPowerWire()
{
	if (IsValid(PowerWire))
	{
		PowerWire->Disconnect();
	}
	PowerWire = nullptr;
}

bool APowerConsumerFacility::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	return PowerProvider != nullptr && IsOn();
}

void APowerConsumerFacility::PreLinkToPowerProvider_Implementation()
{
}

void APowerConsumerFacility::OnLinkToPowerProvider_Implementation(AActor* PowerProviderActor)
{
	LinkedPowerProvider = PowerProviderActor;
	LinkedPowerProviderInterface = Cast<IPowerProviderFacility>(LinkedPowerProvider);
	check(LinkedPowerProviderInterface);
	LinkedPowerProviderInterface->UpdatePowerUsage(PowerConsumption);
	for (AFacilityAddon* Addon : ConnectedAddons)
	{
		Addon->OnFacilityLinkedToPowerProvider();
	}
	UpdatePowerWidgets();

	// PowerWire 연결
	TryConnectPowerWire(PowerProviderActor);
}

void APowerConsumerFacility::PostLinkToPowerProvider_Implementation()
{
}

void APowerConsumerFacility::PreUnlinkFromPowerProvider_Implementation()
{
}

void APowerConsumerFacility::OnUnlinkFromPowerProvider_Implementation()
{
	DisconnectPowerWire();

	if (LinkedPowerProvider)
	{
		LinkedPowerProviderInterface->UpdatePowerUsage(-PowerConsumption);
	}
	LinkedPowerProvider = nullptr;

	if (IsOn())
	{
		TryLinkToNearByPowerProvider();
	}

	UpdatePowerWidgets();
}

void APowerConsumerFacility::PostUnlinkFromPowerProvider_Implementation()
{
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
		LinkedPowerProviderInterface->UnlinkPowerConsumerFacility(this);
	}
}

bool APowerConsumerFacility::TryTurnOn()
{
	if (IsConstructed())
	{
		PreTurnOn();
		bOn = true;
		PostTurnOn();
		return true;
	}
	return false;
}

void APowerConsumerFacility::TurnOff()
{
	PreTurnOff();
	bOn = false;
	PostTurnOff();
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

void APowerConsumerFacility::PreTurnOn_Implementation()
{
}

void APowerConsumerFacility::PostTurnOn_Implementation()
{
	TryLinkToNearByPowerProvider();
	UpdatePowerWidgets();
}

void APowerConsumerFacility::PreTurnOff_Implementation()
{
}

void APowerConsumerFacility::PostTurnOff_Implementation()
{
	UnlinkFromPowerProvider();
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