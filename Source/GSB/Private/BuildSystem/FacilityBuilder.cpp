// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/FacilityBuilder.h"
#include "BuildSystem/FacilityGhostActor.h"
#include "Facility/ConstructibleFacility.h"
#include "Facility/MiningPoint.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "GSBDefines.h"
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
	CancelPreview();
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
	CancelPreview();
	if (MiningFacilityClass)
	{
		FacilityGhost = SpawnFacilityGhost(MiningFacilityClass, true);
		BuildMode = EBuildMode::EBT_MiningFacility;
	}
}

void AFacilityBuilder::PreviewConveyorBelt()
{
	CancelPreview();
	if (ConveyorBeltForwardClass && ConveyorBeltLeftClass && ConveyorBeltRightClass)
	{
		FacilityGhost = SpawnFacilityGhost(ConveyorBeltForwardClass, false);
		BuildMode = EBuildMode::EBT_ConveyorBelt;
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ConveyorBeltClasses가 nullptr입니다."));
	}
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
	if (BuildMode == EBuildMode::EBT_ConveyorBelt)
	{
		BuildFirstConveyorBelt();
	}


	if (IsValid(FacilityGhost))
	{
		FacilityGhost->Destroy();
	}

	if (UGameInstance* GameInst = GetGameInstance())
	{
		if (UGSBFacilitySubsystem* FacilityManager = GameInst->GetSubsystem<UGSBFacilitySubsystem>())
		{
			FacilityManager->SetPowerInfluenceVisibility(false);
		}
	}

	DestroyAllConveyorBeltGhosts();

	BuildMode = EBuildMode::EBT_None;
}

void AFacilityBuilder::RotatePreview()
{
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

AConstructibleFacility* AFacilityBuilder::BuildFacility(TSubclassOf<AConstructibleFacility> FacilityClass, const FTransform& Transform)
{
	if (AConstructibleFacility* Facility = GetWorld()->SpawnActor<AConstructibleFacility>(FacilityClass, Transform))
	{
		Facility->TryBeginConstruction();
		return Facility;
	}
	return nullptr;	
}

void AFacilityBuilder::BuildFirstConveyorBelt()
{
	if (ConveyorBeltGhosts.IsValidIndex(0) && IsValidGeneralFacilityPlace(ConveyorBeltGhosts[0]))
	{
		FTransform BuildTransform = ConveyorBeltGhosts[0]->GetTransform();
		BuildConveyorBeltByChainRelativeDirection(ChainRelativeDirection_LastTick, BuildTransform);
	}
}

void AFacilityBuilder::Tick_GeneralFacilityBuildMode(float DeltaSeconds)
{
	FacilityGhost->SnapActorToGrid(GetActorLocation());
	FacilityGhost->UpdatePlacementDecal(IsValidGeneralFacilityPlace(FacilityGhost));
}

void AFacilityBuilder::Tick_MiningFacilityBuildMode(float DeltaSeconds)
{
	FacilityGhost->SnapActorToGrid(GetActorLocation());
	FacilityGhost->UpdatePlacementDecal(IsValidMiningFacilityPlace(FacilityGhost));
}

void AFacilityBuilder::Tick_ConveyorBeltBuildMode(float DeltaSeconds)
{
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->SnapActorToGrid(GetActorLocation());
		FacilityGhost->UpdatePlacementDecal(IsValidGeneralFacilityPlace(FacilityGhost));
	}
	else if (!ConveyorBeltGhosts.IsEmpty())
	{
		// 방향 평가
		FVector StartForward = ConveyorBeltGhosts[0]->GetActorForwardVector();
		FVector StartToEndVector = GetActorLocation() - ConveyorBeltGhosts[0]->GetLocationOnXYPlane();
		FVector EndDirection = StartToEndVector;
		EndDirection.Normalize();
		double SFoED = StartForward.Dot(EndDirection);
		EChainRelativeDirection ChainRelativeDirection;
		if (SFoED > 0.5)
		{
			ChainRelativeDirection = EChainRelativeDirection::ECRD_Forward;
		}
		else if (SFoED < -0.5)
		{
			ChainRelativeDirection = EChainRelativeDirection::ECRD_Backward;
		}
		else
		{
			FVector StartRight = ConveyorBeltGhosts[0]->GetActorRightVector();
			double SRoED = StartRight.Dot(EndDirection);
			if (SRoED > 0.5f)
			{
				ChainRelativeDirection = EChainRelativeDirection::ECRD_Right;
			}
			else
			{
				ChainRelativeDirection = EChainRelativeDirection::ECRD_Left;
			}
		}

		// Preview 초기화 및 재생성
		if (ChainRelativeDirection != ChainRelativeDirection_LastTick)
		{
			FTransform StartTransform = ConveyorBeltGhosts[0]->GetActorTransform();
			DestroyAllConveyorBeltGhosts();
			switch (ChainRelativeDirection)
			{
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Forward:
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Backward:
			{
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(ConveyorBeltForwardClass, false);
				ConveyorBeltGhosts.Add(Ghost);
				Ghost->SetActorTransform(StartTransform);

				break;
			}
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Right:
			{
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(ConveyorBeltRightClass, false);
				ConveyorBeltGhosts.Add(Ghost);
				Ghost->SetActorTransform(StartTransform);

				break;
			}
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Left:
			{
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(ConveyorBeltLeftClass, false);
				ConveyorBeltGhosts.Add(Ghost);
				Ghost->SetActorTransform(StartTransform);

				break;
			}
			default:
				break;
			}
		}

		// ChainPreview 생성
		double ChainLength = StartToEndVector.Length();
		int GhostsNum = (ChainLength - GRID_CELL_SIZE / 2) / GRID_CELL_SIZE + 1;
		GhostsNum = FMath::Max(1, GhostsNum);
		FVector ChainConveyorBeltLocationOffset;
		FRotator ChainConveyorBeltsRotation;
		switch (ChainRelativeDirection)
		{
		case AFacilityBuilder::EChainRelativeDirection::ECRD_Forward:
			ChainConveyorBeltLocationOffset = ConveyorBeltGhosts[0]->GetActorForwardVector() * GRID_CELL_SIZE;
			ChainConveyorBeltsRotation = ConveyorBeltGhosts[0]->GetActorRotation();
			break;
		case AFacilityBuilder::EChainRelativeDirection::ECRD_Right:
			ChainConveyorBeltLocationOffset = ConveyorBeltGhosts[0]->GetActorRightVector() * GRID_CELL_SIZE;
			ChainConveyorBeltsRotation = ConveyorBeltGhosts[0]->GetActorRotation() + FRotator(0, 90, 0);
			break;
		case AFacilityBuilder::EChainRelativeDirection::ECRD_Left:
			ChainConveyorBeltLocationOffset = -ConveyorBeltGhosts[0]->GetActorRightVector() * GRID_CELL_SIZE;
			ChainConveyorBeltsRotation = ConveyorBeltGhosts[0]->GetActorRotation() + FRotator(0, -90, 0);
			break;
		case AFacilityBuilder::EChainRelativeDirection::ECRD_Backward:
		default:
			ChainConveyorBeltLocationOffset = FVector();
			ChainConveyorBeltsRotation = FRotator();
			GhostsNum = 1;
			break;
		}

		for (int i = ConveyorBeltGhosts.Num() - 1; i >= GhostsNum; --i)
		{
			ConveyorBeltGhosts[i]->Destroy();
			ConveyorBeltGhosts.RemoveAt(i);
		}

		for (int i = ConveyorBeltGhosts.Num(); i < GhostsNum; ++i)
		{
			AFacilityGhostActor* Ghost = SpawnFacilityGhost(ConveyorBeltForwardClass, false);
			Ghost->SetActorLocation(ConveyorBeltGhosts[i - 1]->GetActorLocation() + ChainConveyorBeltLocationOffset);
			Ghost->SetActorRotation(ChainConveyorBeltsRotation);
			ConveyorBeltGhosts.Add(Ghost);
		}

		// Place Validation 평가
		for (AFacilityGhostActor* Ghost : ConveyorBeltGhosts)
		{
			Ghost->UpdatePlacementDecal(IsValidGeneralFacilityPlace(Ghost));
		}
		

		ChainRelativeDirection_LastTick = ChainRelativeDirection;
	}
	else
	{
		CancelPreview();
	}
}

void AFacilityBuilder::ConfirmPlacement_GeneralFacilityBuildMode()
{
	if (FacilityGhost && IsValidGeneralFacilityPlace(FacilityGhost))
	{
		BuildFacility(CurrentGeneralFacilityClass, FacilityGhost->GetTransform());
	}
}

void AFacilityBuilder::ConfirmPlacement_MiningFacilityBuildMode()
{
	if (FacilityGhost && IsValidMiningFacilityPlace(FacilityGhost))
	{
		BuildFacility(MiningFacilityClass, FacilityGhost->GetTransform());
	}
}

void AFacilityBuilder::ConfirmPlacement_ConveyorBeltBuildMode()
{
	if (ConveyorBeltGhosts.Num() > 0)
	{
		BuildFirstConveyorBelt();
		for (int i = 1; i < ConveyorBeltGhosts.Num() - 1; ++i)
		{
			if (IsValidGeneralFacilityPlace(ConveyorBeltGhosts[i]))
			{
				BuildFacility(ConveyorBeltForwardClass, ConveyorBeltGhosts[i]->GetActorTransform());
			}
		}
		if (ConveyorBeltGhosts.Num() == 1)
		{
			PreviewConveyorBelt();
			return;
		}
		FTransform LastGhostTransform = ConveyorBeltGhosts.Last()->GetTransform();
		PreviewConveyorBelt();
		FacilityGhost->SetActorTransform(LastGhostTransform);
	}


	if (IsValid(FacilityGhost))
	{
		ConveyorBeltGhosts.Add(FacilityGhost);
		FacilityGhost = nullptr;
	}
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
			if (UGameInstance* GameInst = GetGameInstance())
			{
				if (UGSBFacilitySubsystem* FacilityManager = GameInst->GetSubsystem<UGSBFacilitySubsystem>())
				{
					FacilityManager->SetPowerInfluenceVisibilityByFacility(TempFacility, true);
				}
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
	Ghost->BoxTraceSingleFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResult, 25);
}

void AFacilityBuilder::DestroyAllConveyorBeltGhosts()
{
	for (AFacilityGhostActor* Ghost : ConveyorBeltGhosts)
	{
		Ghost->Destroy();
	}
	ConveyorBeltGhosts.Empty();
}

void AFacilityBuilder::BuildConveyorBeltByChainRelativeDirection(EChainRelativeDirection ChainRelativeDirection, const FTransform& BuildTransform)
{
	switch (ChainRelativeDirection)
	{
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Forward:
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Backward:
		BuildFacility(ConveyorBeltForwardClass, BuildTransform);
		break;
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Right:
		BuildFacility(ConveyorBeltRightClass, BuildTransform);
		break;
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Left:
		BuildFacility(ConveyorBeltLeftClass, BuildTransform);
		break;
	default:
		break;
	}
}