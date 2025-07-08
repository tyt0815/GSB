// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/MiningFacilityBuilder.h"
#include "BuildSystem/FacilityGhostActor.h"
#include "Facility/MiningPoint.h"

bool AMiningFacilityBuilder::IsVaildPlace(AFacilityGhostActor* Ghost)
{
	FHitResult HitResult;
	TraceGridBoundsInGhostGridBounds(Ghost, HitResult);

	if (AMiningPoint* MiningPoint = Cast<AMiningPoint>(HitResult.GetActor()))
	{
		FVector GhostXYPlaneLocation = Ghost->GetLocationOnXYPlane();
		FVector MiningPointXYPlaneLocation = MiningPoint->GetLocationOnXYPlane();
		return GhostXYPlaneLocation.Equals(MiningPointXYPlaneLocation, 0.001);
	}

	return false;
}
