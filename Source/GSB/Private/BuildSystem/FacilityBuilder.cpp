// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/FacilityBuilder.h"
#include "BuildSystem/FacilityGhostActor.h"
#include "Facility/ConstructibleFacility.h"
#include "Facility/MiningPoint.h"
#include "Facility/MiningFacility.h"
#include "Facility/ConveyorBelt.h"
#include "HUDs/GSBFacilityQuickSlotList.h"
#include "HUDs/GSBConstructableFacilityListWindow.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "SubSystems/GSBWindowSubsystem.h"
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

void AFacilityBuilder::BeginPlay()
{
	Super::BeginPlay();
}

void AFacilityBuilder::PreviewFacilityAt(int32 Index)
{
	PreviewFacilityByFacilityData(FacilityPreviewQuickSlot[Index]);
}

void AFacilityBuilder::PreviewFacilityByFacilityData(UGSBFacilityDataAsset* FacilityData)
{
	if (IsValid(FacilityData))
	{
		CurrentPreviewFacilityData = FacilityData;
		if (UClass* FacilityClass = FacilityData->FacilityClass)
		{
			if (!FacilityClass->IsChildOf<AConstructibleFacility>())
			{
				return;
			}
			else if (FacilityClass->IsChildOf<AMiningFacility>())
			{
				PreviewMiningFacility();
			}
			else if (FacilityClass->IsChildOf<AConveyorBelt>())
			{
				PreviewConveyorBelt();
			}
			else
			{
				PreviewGeneralFacility(FacilityData);
			}
		}

		SetPowerInfluenceVisibilityByFacilityData(FacilityData, true);
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

	SetPowerInfluenceVisibility(false);

	DestroyAllConveyorBeltGhosts();

	BuildMode = EBuildMode::EBT_None;
}

void AFacilityBuilder::RotatePreview()
{
	AddActorWorldRotation(FRotator(0, 90, 0));
	if (IsValid(FacilityGhost))
	{
		FacilityGhost->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}

void AFacilityBuilder::SetFacilityQuickSlot(UGSBFacilityDataAsset* FacilityData, int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	FacilityPreviewQuickSlot[Index] = FacilityData;
	if (IsValid(FacilityQuickSlotListWidget))
	{
		FacilityQuickSlotListWidget->UpdateFacilityQuickSlot(FacilityData, Index);
	}
}

void AFacilityBuilder::ToggleBuildableFacilityList()
{
	if (UGSBWindowSubsystem* WindowManager = UGSBWindowSubsystem::Get(this))
	{
		WindowManager->ToggleWindow(ConstructableFacilityListWindow, TEXT("ConstructableFacilityListWindow"), TEXT("ConstructableFacilityListWindow"));
		if (IsValid(ConstructableFacilityListWindow))
		{
			ConstructableFacilityListWindow->SetFacilityBuilder(this);
			ConstructableFacilityListWindow->UpdateList();
		}
	}
}

void AFacilityBuilder::PreviewGeneralFacility(UGSBFacilityDataAsset* FacilityData)
{
	CancelPreview();
	if (IsValid(FacilityData)) 
	{
		if (UClass* FacilityClass = FacilityData->FacilityClass)
		{
			if (FacilityClass->IsChildOf<AConstructibleFacility>())
			{
				FacilityGhost = SpawnFacilityGhost(FacilityClass);
				BuildMode = EBuildMode::EBT_GeneralFacility;
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("FacilityClass 클래스는 AConstructibleFacility를 상속한 클래스이어야 합니다."));
			}
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("FacilityClass가 nullptr입니다."));
		}
	}
}

void AFacilityBuilder::PreviewMiningFacility()
{
	CancelPreview();
	if (IsValid(MiningFacilityData))
	{
		if (UClass* FacilityClass = MiningFacilityData->FacilityClass)
		{
			if (FacilityClass->IsChildOf<AMiningFacility>())
			{
				FacilityGhost = SpawnFacilityGhost(FacilityClass);
				BuildMode = EBuildMode::EBT_MiningFacility;
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("FacilityClass는 AMiningFacility를 상속한 클래스이어야 합니다."));
			}
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("FacilityClass가 nullptr 입니다."));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("MiningFacilityData가 nullptr 입니다."));
	}
}

void AFacilityBuilder::PreviewConveyorBelt()
{
	CancelPreview();
	if (IsValid(ConveyorBeltForwardData) && IsValid(ConveyorBeltLeftData) && IsValid(ConveyorBeltRightData))
	{
		UClass* ForwardClass = ConveyorBeltForwardData->FacilityClass;
		UClass* LeftClass = ConveyorBeltLeftData->FacilityClass;
		UClass* RightClass = ConveyorBeltRightData->FacilityClass;
		if (ForwardClass->IsChildOf<AConveyorBelt>() && LeftClass->IsChildOf<AConveyorBelt>() && RightClass->IsChildOf<AConveyorBelt>())
		{
			FacilityGhost = SpawnFacilityGhost(ForwardClass);
			BuildMode = EBuildMode::EBT_ConveyorBelt;
		}
		
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("ConveyorBeltForwardDatas가 nullptr입니다."));
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
		FVector StartToEndVector = GetActorLocation() - ConveyorBeltGhosts[0]->GetLocationOnBottomXYPlane();
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
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(*(ConveyorBeltForwardData->FacilityClass));
				ConveyorBeltGhosts.Add(Ghost);
				Ghost->SetActorTransform(StartTransform);

				break;
			}
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Right:
			{
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(*(ConveyorBeltRightData->FacilityClass));
				ConveyorBeltGhosts.Add(Ghost);
				Ghost->SetActorTransform(StartTransform);

				break;
			}
			case AFacilityBuilder::EChainRelativeDirection::ECRD_Left:
			{
				AFacilityGhostActor* Ghost = SpawnFacilityGhost(*(ConveyorBeltLeftData->FacilityClass));
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
		int GhostsNum = (ChainLength - GRID_CELL_SIZE / 2) / GRID_CELL_SIZE + 2;
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
			AFacilityGhostActor* Ghost = SpawnFacilityGhost(*(ConveyorBeltForwardData->FacilityClass));
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
		BuildFacility(*(CurrentPreviewFacilityData->FacilityClass), FacilityGhost->GetTransform());
		SetPowerInfluenceVisibilityByFacilityData(CurrentPreviewFacilityData, true);
	}
}

void AFacilityBuilder::ConfirmPlacement_MiningFacilityBuildMode()
{
	if (FacilityGhost && IsValidMiningFacilityPlace(FacilityGhost))
	{
		BuildFacility(*(MiningFacilityData->FacilityClass), FacilityGhost->GetTransform());
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
				BuildFacility(*(ConveyorBeltForwardData->FacilityClass), ConveyorBeltGhosts[i]->GetActorTransform());
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
	TraceBuildBlockerInGhostGridBounds(Ghost, HitResult);

	return !IsValid(HitResult.GetActor());
}

bool AFacilityBuilder::IsValidMiningFacilityPlace(AFacilityGhostActor* Ghost)
{
	FHitResult HitResult;
	TraceBuildBlockerInGhostGridBounds(Ghost, HitResult);

	if (AMiningPoint* MiningPoint = Cast<AMiningPoint>(HitResult.GetActor()))
	{
		FVector GhostXYPlaneLocation = Ghost->GetLocationOnBottomXYPlane();
		FVector MiningPointXYPlaneLocation = MiningPoint->GetLocationOnBottomXYPlane();
		return GhostXYPlaneLocation.Equals(MiningPointXYPlaneLocation, 0.001);
	}

	return false;
}

void AFacilityBuilder::SetPowerInfluenceVisibilityByFacilityData(UGSBFacilityDataAsset* FacilityData, bool bVisible)
{
	if (IsValid(FacilityData))
	{
		SetPowerInfluenceVisibilityByFacilityClass(FacilityData->FacilityClass, bVisible);
	}
	
}

void AFacilityBuilder::SetPowerInfluenceVisibilityByFacilityClass(UClass* FacilityClass, bool bVisible)
{
	if (IsValid(FacilityClass))
	{
		SetPowerInfluenceVisibilityByFacility(Cast<AFacility>(FacilityClass->GetDefaultObject()), bVisible);
	}
}

void AFacilityBuilder::SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisible)
{
	if (IsValid(Facility))
	{
		if (UGameInstance* GameInst = GetGameInstance())
		{
			if (UGSBFacilitySubsystem* FacilityManager = GameInst->GetSubsystem<UGSBFacilitySubsystem>())
			{
				FacilityManager->SetPowerInfluenceVisibilityByFacility(Facility, bVisible);
			}
		}
	}
}

void AFacilityBuilder::SetPowerInfluenceVisibility(bool bVisible)
{
	if (UGameInstance* GameInst = GetGameInstance())
	{
		if (UGSBFacilitySubsystem* FacilityManager = GameInst->GetSubsystem<UGSBFacilitySubsystem>())
		{
			FacilityManager->SetPowerInfluenceVisibility(bVisible);
		}
	}
}

AFacilityGhostActor* AFacilityBuilder::SpawnFacilityGhost(const TSubclassOf<AConstructibleFacility>& FacilityClass)
{
	UWorld* World = GetWorld();
	if (AConstructibleFacility* TempFacility = World->SpawnActor<AConstructibleFacility>(FacilityClass))
	{
		AFacilityGhostActor* Ghost = World->SpawnActor<AFacilityGhostActor>();

		Ghost->BuildGhostPreview(TempFacility);
		Ghost->SetActorRotation(GetActorRotation());
		Ghost->SnapActorToGrid(GetActorLocation());

		TempFacility->Destroy();

		return Ghost;
	}
	
	return nullptr;
}

void AFacilityBuilder::TraceBuildBlockerInGhostGridBounds(AFacilityGhostActor* Ghost, FHitResult& HitResult)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
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
		BuildFacility(*(ConveyorBeltForwardData->FacilityClass), BuildTransform);
		break;
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Right:
		BuildFacility(*ConveyorBeltRightData->FacilityClass, BuildTransform);
		break;
	case AFacilityBuilder::EChainRelativeDirection::ECRD_Left:
		BuildFacility(*ConveyorBeltLeftData->FacilityClass, BuildTransform);
		break;
	default:
		break;
	}
}