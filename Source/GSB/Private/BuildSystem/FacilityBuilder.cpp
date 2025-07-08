// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/FacilityBuilder.h"
#include "BuildSystem/FacilityGhostActor.h"
#include "Facility/ConstructibleFacility.h"
#include "Facility/MiningPoint.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AFacilityBuilder::AFacilityBuilder()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFacilityBuilder::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	switch (BuildMode)
	{
	case EBuildMode::EBT_GeneralFacility:
		Tick_GeneralFacilityBuildMode(DeltaSeconds);
		break;
	case EBuildMode::EBT_MiningFacility:
		Tick_MiningFacilityBuildMode(DeltaSeconds);
		break;
	case EBuildMode::EBT_ConveyorBelt:
		Tick_ConveyorBeltBuildMode(DeltaSeconds);
		break; 
	case EBuildMode::EBT_None:
	default:
		break;
	}
}

void AFacilityBuilder::PreviewGeneralFacility(const FName& FacilityName)
{
	if (GeneralFacilityClasses.Contains(FacilityName))
	{
		const TSubclassOf<AConstructibleFacility>& FacilityClass = *GeneralFacilityClasses.Find(FacilityName);
		if (FacilityClass)
		{
			CurrentGeneralFacilityClass = FacilityClass;
			FacilityGhost = SpawnFacilityGhost(CurrentGeneralFacilityClass, true);
			BuildMode = EBuildMode::EBT_GeneralFacility;
		}
		else
		{
			TRACE_SCREEN_LOG(FacilityName.ToString() + TEXT("클래스가 nullptr입니다."));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(FacilityName.ToString() + TEXT("가 존재하지 않습니다."));
	}
}

void AFacilityBuilder::PreviewMiningFacility()
{
	if (MiningFacilityClass)
	{
		FacilityGhost = SpawnFacilityGhost(MiningFacilityClass, true);
		BuildMode = EBuildMode::EBT_MiningFacility;
	}
}

void AFacilityBuilder::PreviewConveyorBelt()
{
	// TODO
}

void AFacilityBuilder::ConfirmFacilityPlacement()
{
	switch (BuildMode)
	{
	case EBuildMode::EBT_GeneralFacility:
		ConfirmPlacement_GeneralFacilityBuildMode();
		break;
	case EBuildMode::EBT_MiningFacility:
		ConfirmPlacement_MiningFacilityBuildMode();
		break;
	case EBuildMode::EBT_ConveyorBelt:
		ConfirmPlacement_ConveyorBeltBuildMode();
		break;
	case EBuildMode::EBT_None:
	default:
		break;
	}
}

void AFacilityBuilder::CancelPreview()
{
	switch (BuildMode)
	{
	case EBuildMode::EBT_GeneralFacility:
	case EBuildMode::EBT_MiningFacility:
		CancelPreview_GeneralAndMiningFacilityBuildMode();
		break;
	case EBuildMode::EBT_ConveyorBelt:
		CancelPreview_ConveyorBeltBuildMode();
		break;
	case EBuildMode::EBT_None:
	default:
		break;
	}
}

AConstructibleFacility* AFacilityBuilder::BuildFacility(TSubclassOf<AConstructibleFacility> FacilityClass, FVector Location)
{
	if (AConstructibleFacility* Facility = GetWorld()->SpawnActor<AConstructibleFacility>(FacilityClass))
	{
		Facility->SetActorLocation(Location);
		Facility->BeginConstruction();

		return Facility;
	}
	return nullptr;	
}

void AFacilityBuilder::Tick_GeneralFacilityBuildMode(float DeltaSeconds)
{
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->SnapActorToGrid(GetActorLocation());
		FacilityGhost->UpdatePlacementDecal(IsValidGeneralFacilityPlace(FacilityGhost));
	}
}

void AFacilityBuilder::Tick_MiningFacilityBuildMode(float DeltaSeconds)
{
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->SnapActorToGrid(GetActorLocation());
		FacilityGhost->UpdatePlacementDecal(IsValidMiningFacilityPlace(FacilityGhost));
	}
}

void AFacilityBuilder::Tick_ConveyorBeltBuildMode(float DeltaSeconds)
{
	// TODO
}

void AFacilityBuilder::ConfirmPlacement_GeneralFacilityBuildMode()
{
	if (FacilityGhost && IsValidGeneralFacilityPlace(FacilityGhost))
	{
		BuildFacility(CurrentGeneralFacilityClass, FacilityGhost->GetActorLocation());
	}
}

void AFacilityBuilder::ConfirmPlacement_MiningFacilityBuildMode()
{
	if (FacilityGhost && IsValidMiningFacilityPlace(FacilityGhost))
	{
		BuildFacility(MiningFacilityClass, FacilityGhost->GetActorLocation());
	}
}

void AFacilityBuilder::ConfirmPlacement_ConveyorBeltBuildMode()
{
	// TODO
}

void AFacilityBuilder::CancelPreview_GeneralAndMiningFacilityBuildMode()
{
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->Destroy();
	}

	if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
	{
		GameInst->SetPowerInfluenceVisibility(false);
	}
}

void AFacilityBuilder::CancelPreview_ConveyorBeltBuildMode()
{
	// TODO
}

bool AFacilityBuilder::IsValidGeneralFacilityPlace(AFacilityGhostActor* Ghost)
{
	FHitResult HitResult;
	TraceGridBoundsInGhostGridBounds(Ghost, HitResult);

	return !IsValid(HitResult.GetActor());
}

bool AFacilityBuilder::IsValidMiningFacilityPlace(AFacilityGhostActor* Ghost)
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

AFacilityGhostActor* AFacilityBuilder::SpawnFacilityGhost(const TSubclassOf<AConstructibleFacility>& FacilityClass, bool bVisiblePowerInfluenceArea)
{
	UWorld* World = GetWorld();
	if (AConstructibleFacility* TempFacility = World->SpawnActor<AConstructibleFacility>(FacilityClass))
	{
		if (bVisiblePowerInfluenceArea)
		{
			if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
			{
				GameInst->SetPowerInfluenceVisibilityByFacility(TempFacility, true);
			}
		}

		AFacilityGhostActor* Ghost = World->SpawnActor<AFacilityGhostActor>();

		Ghost->BuildGhostPreview(TempFacility);
		Ghost->SnapActorToGrid(GetActorLocation());

		TempFacility->Destroy();

		return Ghost;
	}
	
	return nullptr;
}

void AFacilityBuilder::TraceGridBoundsInGhostGridBounds(AFacilityGhostActor* Ghost, FHitResult& HitResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Ghost);
	Ghost->BoxTraceSingleFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResult, 50);
}