// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/MiningFacility.h"
#include "Facility/MiningPoint.h"
#include "Facility/Addon/OutputPort.h"
#include "Components/ItemStorageComponent.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "DebugHeader.h"

AMiningFacility::AMiningFacility()
{
	PrimaryActorTick.bCanEverTick = true;

	FacilityName = TEXT("채굴기");
}

void AMiningFacility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsActivate() && IsValid(OccupiedMiningPoint))
	{
		if (CurrentMiningTime >= OccupiedMiningPoint->GetMiningTime())
		{
			if (ConnectedOutputPort->TryCreateAndSendItemCrate(OccupiedMiningPoint->GetMineableItem()))
			{
				CurrentMiningTime = 0;
			}
		}
		else
		{
			CurrentMiningTime += DeltaSeconds;
		}
	}
}

void AMiningFacility::OnUnlinkFromPowerProvider_Implementation()
{
	Super::OnUnlinkFromPowerProvider_Implementation();

}

void AMiningFacility::RegisterOutputPort(AOutputPort* OutputPort)
{
	ConnectedOutputPort = OutputPort;
}

void AMiningFacility::BeginPlay()
{
	Super::BeginPlay();

	TryOccupyMiningPoint();
}

bool AMiningFacility::TryOccupyMiningPoint()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnores;
	TArray<FHitResult> HitResults;
	BoxTraceMultiFromGridBoundsForObjects(ObjectTypes, ActorsToIgnores, HitResults, 10);

	for (const FHitResult& HitResult : HitResults)
	{
		OccupiedMiningPoint = Cast<AMiningPoint>(HitResult.GetActor());
		if (OccupiedMiningPoint)
		{
			return true;
		}
	}

	return false;
}
