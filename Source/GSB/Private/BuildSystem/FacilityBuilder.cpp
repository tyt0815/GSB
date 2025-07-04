// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/FacilityBuilder.h"
#include "Facility/ConveyorBelt.h"
#include "Facility/MiningFacility.h"
#include "Facility/MiningPoint.h"
#include "Components/DecalComponent.h"
#include "Components/GridSizeBoxComponent.h"
#include "GSBGameInstance.h"
#include "GSBDefines.h"
#include "Components/GhostPreviewComponent.h"
#include "DebugHeader.h"

AFacilityBuilder::AFacilityBuilder()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_PostUpdateWork);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(GetRootComponent());
	DecalComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DecalComponent->SetVisibility(false);

	GhostPreviewComponent = CreateDefaultSubobject<UGhostPreviewComponent>(TEXT("GhostPreview"));
	GhostPreviewComponent->SetupAttachment(GetRootComponent());
}

void AFacilityBuilder::Tick(float Delta)
{
	// TODO: Decal Material 설정
	
	if (IsValid(PreviewFacilityClass))
	{
		DecalComponent->SetWorldLocation(GetActorLocation());
		if (IsValidPlace())
		{
			DecalComponent->SetMaterial(0, ValidPlacementDecal);
		}
		else
		{
			DecalComponent->SetMaterial(0, InvalidPlacementDecal);
		}
	}
}

void AFacilityBuilder::PreviewFacility(TSubclassOf<AConstructibleFacility> FacilityClass)
{
	UWorld* World = GetWorld();
	CancelFacilityPreview();
	if (World && FacilityClass)
	{
		PreviewFacilityClass = FacilityClass;
		AConstructibleFacility* TempFacility = World->SpawnActor<AConstructibleFacility>(PreviewFacilityClass);

		GhostPreviewComponent->BuildGhostPreview(TempFacility);
		GridBoundsComponent->SetBoxExtent(TempFacility->GetGridBoundsComponent()->GetScaledBoxExtent());

		DecalComponent->SetVisibility(true);
		FVector FacilityPreviewBoxExtent = TempFacility->GetGridBoundsComponent()->GetScaledBoxExtent();
		DecalComponent->DecalSize = FVector(FacilityPreviewBoxExtent.Z + GRID_CELL_SIZE, FacilityPreviewBoxExtent.Y, FacilityPreviewBoxExtent.X);

		SetPowerInfluenceVisibilityByFacilityClass(FacilityClass, true);

		if (Cast<class AConveyorBelt>(TempFacility))
		{
			BuildCategory = EBuildCategory::EBC_ConveyorBelt;
		}
		else if (Cast<class AMiningFacility>(TempFacility))
		{
			BuildCategory = EBuildCategory::EBC_MiningFacility;
		}
		else
		{
			BuildCategory = EBuildCategory::EBC_GeneralFacility;
		}

		TempFacility->Destroy();
	}
}

void AFacilityBuilder::ConfirmFacilityPlacement()
{
	UWorld* World = GetWorld();
	if (IsValid(PreviewFacilityClass) && IsValid(World))
	{
		
		AConstructibleFacility* Facility = World->SpawnActor<AConstructibleFacility>(PreviewFacilityClass, GetActorTransform());

		if (IsValid(Facility))
		{
			Facility->BeginConstruction();
		}
	}
	CancelFacilityPreview();
}

void AFacilityBuilder::CancelFacilityPreview()
{
	if (IsValid(PreviewFacilityClass))
	{
		SetPowerInfluenceVisibilityByFacilityClass(PreviewFacilityClass, false);
	}
	GhostPreviewComponent->ClearGhostPreviewComponents();
	DecalComponent->SetVisibility(false);
	PreviewFacilityClass = nullptr;
}

bool AFacilityBuilder::IsPlacingFacility() const
{
	return PreviewFacilityClass != nullptr;
}

bool AFacilityBuilder::IsValidPlace() const
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnores;
	FHitResult HitResult;
	BoxTraceSingleFromGridBoundsForObjects(ObjectTypes, ActorsToIgnores, HitResult, 10);
	if (BuildCategory == EBuildCategory::EBC_MiningFacility)
	{
		AMiningPoint* MiningPoint = Cast <AMiningPoint>(HitResult.GetActor());
		if (IsValid(MiningPoint))
		{
			FVector LocationXY = GetLocationOnXYPlane();
			FVector MiningPointXY = MiningPoint->GetLocationOnXYPlane();
			SCREEN_LOG(1, LocationXY.ToString())
			SCREEN_LOG(2, MiningPointXY.ToString())
			if (LocationXY.Equals(MiningPointXY))
			{
				return true;
			}
		}
	}
	else
	{
		if (!HitResult.GetActor())
		{
			return true;
		}
	}

	return false;
}

void AFacilityBuilder::SetPowerInfluenceVisibilityByFacility(AFacility* Facility, bool bVisibility)
{
	if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
	{
		GameInst->SetPowerInfluenceVisibilityByFacility(Facility, bVisibility);
	}
}

void AFacilityBuilder::SetPowerInfluenceVisibilityByFacilityClass(TSubclassOf<AConstructibleFacility> FacilityClass, bool bVisibility)
{
	if (AFacility* DefaultFacility = FacilityClass->GetDefaultObject<AFacility>())
	{
		SetPowerInfluenceVisibilityByFacility(DefaultFacility, bVisibility);
	}
}
