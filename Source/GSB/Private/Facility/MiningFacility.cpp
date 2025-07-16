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
}

void AMiningFacility::CompleteConstruction_Implementation()
{
	Super::CompleteConstruction_Implementation();
	TryOccupyMiningPoint();
}

void AMiningFacility::Tick_OnOperating(float DeltaSeconds)
{
	Super::Tick_OnOperating(DeltaSeconds);
	if (!IsValid(OccupiedMiningPoint))
	{
		return;
	}

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

void AMiningFacility::OnUnlinkFromPowerProvider_Implementation()
{
	Super::OnUnlinkFromPowerProvider_Implementation();

}

void AMiningFacility::RegisterOutputPort(AOutputPort* OutputPort)
{
	ConnectedOutputPort = OutputPort;
}

FName AMiningFacility::GetMiningItemName() const
{
	return FName(OccupiedMiningPoint->GetMineableItem()->Name.ToString());
}

float AMiningFacility::GetMiningTime() const
{
	return OccupiedMiningPoint->GetMiningTime();
}

float AMiningFacility::GetMiningProgress() const
{
	return FMath::Clamp(CurrentMiningTime / OccupiedMiningPoint->GetMiningTime(), 0, 1);
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
